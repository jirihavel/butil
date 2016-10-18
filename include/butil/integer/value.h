#ifndef BUTIL_INTEGER_VALUE_H_INCLUDED
#define BUTIL_INTEGER_VALUE_H_INCLUDED

#include <butil/enum_flags.h>
#include <butil/integer/interval.h>
#include <butil/integer/intset.h>

#include <type_traits>
#include <ostream>

namespace butil {
namespace integer {

/** \brief Represent possible nonstandard conditions.
 */
enum class value_flags
{
    // Value is valid
    none = 0x0,

    // Range error
    // - result is finite number, but not representable
    // - arighmetic overflow
    // - conversion to smaller type (here positive number can lead to negative_overflow)
    positive_overflow = 0x1,
    negative_overflow = 0x2,
    overflow = positive_overflow | negative_overflow,

    // Pole error
    // - result has only limit
    // - divide by zero
    positive_infinity = 0x4,
    negative_infinity = 0x8,
    infinity = positive_infinity | negative_infinity,

    // Domain error
    // - result without a limit
    // - 0/0, ...
    domain_error = 0x10
};
BUTIL_ENUM_FLAGS(value_flags)

namespace detail {

/* \brief Hack to put asserts in constexpr constructors.
 */
struct ctor_sink
{
    constexpr ctor_sink() noexcept = default;
    template<typename T>
    constexpr explicit ctor_sink(T const &) noexcept {}
};

/** \brief Stores value flags.
 *
 * \param FlagMask specifies expected subset of flags.
 *
 * Specialized for cases when no flags are expected.
 * Checks (assert) that flags are subset of the allowed ones.
 */
template<value_flags FlagMask>
class value_base
{
    static constexpr bool flags_are_expected(value_flags flags) noexcept
    {
        return ((~FlagMask)&flags) == value_flags::none;
    }
public :
    template<value_flags Flags>
    constexpr explicit value_base(std::integral_constant<value_flags, Flags>) noexcept
        : m_flags(Flags)
    {
        static_assert(flags_are_expected(Flags), "");
    }

    constexpr explicit value_base(value_flags flags) noexcept
        : m_flags((assert(flags_are_expected(flags)), flags))
    {}

    constexpr value_flags flags() const noexcept
    {
        return m_flags;
    }
private :
    value_flags m_flags;
};

template<>
class value_base<value_flags::none>
    : private ctor_sink
{
public :
    template<value_flags Flags>
    constexpr explicit value_base(std::integral_constant<value_flags, Flags>) noexcept
    {
        static_assert(Flags == value_flags::none, "");
    }

    constexpr explicit value_base(value_flags flags) noexcept
        : ctor_sink((assert(flags == value_flags::none), flags))
    {}

    constexpr value_flags flags() const noexcept
    {
        return value_flags::none;
    }
};

}//namespace detail

/** \brief Integer value with associated set and optional flags.
 *
 * Holds integer value and the set it belongs to.
 *
 * Can additionally hold flags to indicate error condition.
 */
template<
    typename Set,
    value_flags FlagMask = value_flags::none,
    typename Int = typename intset_type<Set>::least
    >
class intset_value : private detail::value_base<FlagMask>
{
    using Base = detail::value_base<FlagMask>;

    static_assert(std::is_integral<Int>::value, "");
    static_assert(interval_includes<Int, Set>::value, "");

    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & out, intset_value self)
    {
        return out << +self.m_value << " in [" << +self.min << ", " << +self.max << ']';
    }
public :
    using intset_type = Set;
    using value_type = Int;

    static constexpr value_type min = intset_traits<Set>::min;
    static constexpr value_type max = intset_traits<Set>::max;

    static constexpr value_flags flag_mask = FlagMask;
    using Base::flags;
    
    /** \brief Construct value from an intset.
     *
     * TODO : if range_error is disabled, U must be subset of Set
     */
    template<typename X
//        typename = std::enable_if_t<interval_includes<Set, U>::value || !!(Flags & value_flags::range_error)>
        >
    explicit constexpr intset_value(X x, value_flags f = value_flags::none) noexcept
        : Base(!!f ? f : (interval_contains<Set>(x) ? value_flags::none : value_flags::overflow))
        , m_value(intset_traits<X>::value(x)) {}

    ~intset_value() noexcept = default;
    intset_value(intset_value const &) noexcept = default;
    intset_value & operator=(intset_value const &) noexcept = default;

    constexpr value_type unsafe_value() const noexcept
    {
        return m_value;
    }

    constexpr value_type value() const noexcept
    {
        return assert(flags() == value_flags::none), unsafe_value();
    }

#define BUTIL_SAFE_OPERATOR(Type) \
    operator Type() const noexcept { \
        static_assert(interval_includes<Type, Set>::value, ""); \
        static_assert(FlagMask == value_flags::none, ""); \
        return this->value(); }
    BUTIL_SAFE_OPERATOR(char)
    BUTIL_SAFE_OPERATOR(signed char)
    BUTIL_SAFE_OPERATOR(unsigned char)
    BUTIL_SAFE_OPERATOR(signed short)
    BUTIL_SAFE_OPERATOR(unsigned short)
    BUTIL_SAFE_OPERATOR(signed int)
    BUTIL_SAFE_OPERATOR(unsigned int)
    BUTIL_SAFE_OPERATOR(signed long)
    BUTIL_SAFE_OPERATOR(unsigned long)
    BUTIL_SAFE_OPERATOR(signed long long)
    BUTIL_SAFE_OPERATOR(unsigned long long)
#undef BUTIL_SAFE_OPERATOR
private :
    value_type m_value;
};

namespace detail {

template<class...> using void_t = void;

template<typename Set, typename = void_t<>>
struct intset_flags_impl
{
    constexpr value_flags operator()(Set) const noexcept { return value_flags::none; }
};

template<typename Set>
struct intset_flags_impl<Set, void_t<decltype(std::declval<Set>().flags())>>
{
    constexpr value_flags operator()(Set x) const noexcept { return x.flags(); }
};

}//namespace detail

template<typename IntSet>
struct intset_flag_traits
{
    static constexpr value_flags flags(IntSet const & x) noexcept
    {
        return value_flags::none;
    }
};

template<typename Set, value_flags Mask, typename Int>
struct intset_flag_traits<intset_value<Set, Mask, Int>>
{
    static constexpr value_flags flags(intset_value<Set, Mask, Int> const & x) noexcept
    {
        return x.flags();
    }
};

/*
template<typename IntSet>
constexpr value_flags intset_flags(IntSet x) noexcept
{
    return detail::intset_flags_impl<IntSet>()(x);
}
*/
namespace detail {

template<typename T>
struct is_intset_value : std::false_type {};
template<typename S, value_flags F, typename I>
struct is_intset_value<intset_value<S, F, I>> : std::true_type {};

}//namespace detail

/** \brief Cast value to other intset.
 *
 * \return Converted value 
 * value -> value / overflow
 * other -> domain_error
 * TODO : expand/configure
 *
 * TODO : positive/negative overflow
 */
template<typename Dst, typename Set, value_flags Flg, typename Int>
constexpr auto value_cast(intset_value<Set, Flg, Int> src) noexcept
{
    // All input flags are preserved + check whether overflow can happen
    using Ret = intset_value<Dst, 
        (interval_includes<Dst, Set>::value ? Flg : Flg | value_flags::overflow)>;
    // Pass flags through or check for overflow
    return Ret(src.unsafe_value(),
            src.flags() == value_flags::none
                ? (interval_contains<Dst>(src) ? value_flags::none : value_flags::overflow)
                : src.flags()
            );
}

template<typename Dst, typename Int, typename = std::enable_if_t<!detail::is_intset_value<Int>::value>>
constexpr auto value_cast(Int src) noexcept
{
    return value_cast<Dst>(intset_value<Int, value_flags::none, typename intset_traits<Int>::value_type>(src));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_VALUE_H_INCLUDED
