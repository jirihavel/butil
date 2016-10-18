#ifndef BUTIL_SUBTYPE_H_INCLUDED
#define BUTIL_SUBTYPE_H_INCLUDED

#include <butil/cast.h>
#include <butil/fail_fast.h>
#include <butil/pun.h>
#include <butil/underlying_value.h>

#include <type_traits>
#include <utility>

// uses :
//
// - function param with prereq
//  - convertible from anything
//  - used asserts declared in function type
//
// - tagged type
//  - no checking
//  - no implicit conversion
//  - named parameters

namespace butil {

/** \brief Empty subtype base.
 * 
 * Every subtype must inherit from this type, it is used for detection.
 */
struct subtype_base {};

struct constant_init_tag {};

enum class conv_level
{
    disable = 0,
    enable = 1,
    implicit = 2
};

struct subtype_traits_base
{
    /** \brief Check constructor parameters before construction.
     *
     * Predicate::operator() must be able to handle them.
     */
    static constexpr bool enforce_preconditions = true;

    /** \brief Check object after construction.
     *
     * The construction can alter the value but the predicate for Type only is simpler.
     */
    static constexpr bool enforce_invariants = !NDEBUG_DEFINED;

    /** \brief How to handle unsafe construction (from something different than subtype).
     *
     * void f(Object x);
     *
     * implicit :
     *  f(1); // compiles
     * enable :
     *  f(1); // fails
     *  f(Object(1)); // compiles
     * disable :
     *  Object(1); // fails
     */
    static constexpr conv_level unsafe_construct = conv_level::disable;

    /** \brief How to handle unsafe conversion from different subtype.
     */
    static constexpr conv_level unsafe_convert = conv_level::disable;

    /** \brief Allows customizing subtype by CRTP
     */
    template<typename Derived>
    using base = subtype_base;
};

template<typename Type, typename Predicate>
struct subtype_traits : subtype_traits_base {};

template<typename Dst, typename ... Args>
struct subtype_construction_traits
{
    using dst_predicate = typename Dst::predicate_type;
    using dst_traits = typename Dst::traits_type;

    using safe = std::false_type;

    static constexpr conv_level level = conv_level::disable;
/*
    static constexpr conv_level ulevel =
        dst_traits::unsafe_construct != conv_level::disable
        ? dst_traits::unsafe_construct
        : std::is_constructible<dst_predicate, Args...>::value
            ? conv_level::enable : conv_level::disable;

    static constexpr conv_level conv = safe::value ? conv_level::implicit : ulevel;
*/
};

namespace detail {

template<typename ... Args>
struct is_subtype : std::false_type {};

template<typename T>
struct is_subtype<T> : std::is_base_of<subtype_base, std::decay_t<T>> {};

template<bool Ok, typename Dst, typename Src>
struct subtype_conversion_impl {};

template<typename Dst, typename Src>
struct subtype_conversion_impl<true, Dst, Src>
{
    using dst_predicate = typename Dst::predicate_type;
    using src_predicate = typename Src::predicate_type;

    using safe = std::is_convertible<src_predicate, dst_predicate>;

    static constexpr conv_level ulevel =
        Dst::traits_type::unsafe_convert != conv_level::disable
        ? Dst::traits_type::unsafe_convert
        : std::is_constructible<dst_predicate, src_predicate const &>::value
            ? conv_level::enable : conv_level::disable;

    static constexpr conv_level conv = safe::value ? conv_level::implicit : ulevel;
};

template<typename Dst, typename Src>
constexpr auto copy_if_possible(Src const & src)
    -> std::enable_if_t<std::is_constructible<Dst, Src const &>::value, Dst>
{
    return Dst(src);
}

template<typename Dst, typename Src>
constexpr auto copy_if_possible(Src const &)
    -> std::enable_if_t<!std::is_constructible<Dst, Src const &>::value, Dst>
{
    return Dst();
}

}//namespace detail

template<typename Dst, typename Src>
struct subtype_conversion : detail::subtype_conversion_impl<detail::is_subtype<Src>::value, Dst, std::decay_t<Src>> {};

/** \brief Subtype limited by a predicate.
 *
 * T is the supertype :
 * - DefaultConstructible
 *   ? co když není?, co když T() nesplňuje predikát?
 * - CopyConstructible
 *   ? nestačí move constructible?
 * - CopyAssignable
 *   ? pokud není, subtyp nepůjde kopírovat
 * - Destructible
 *
 * Predicate limits the subtype :
 * - Predicate
 *   - (T const & -> bool) for checking constructed value
 *   - (Args && ... -> bool) for checking constructor params (default)
 * - DefaultConstructible
 *   ? stačí případně copy constructible?
 * - CopyConstructible
 * - CopyAssignable ?
 * - Destructible
 *
 * ???
 * - constexpr
 * - propagovat funktor ano/ne
 * - implicitní konverze na subtyp/supertyp ano/ne
 *
 */
template<typename T, typename Predicate,
    typename Traits = subtype_traits<T, Predicate>>
class subtype
    : public Traits::template base<subtype<T, Predicate, Traits>>
{
    class predicate_wrapper : protected Predicate
    {
    protected :
        /** \brief Construct predicate and do not check ctor preconditions.
         */
        template<typename ... Args>
        constexpr predicate_wrapper(std::true_type, Predicate const & p, Args && ...)
            : Predicate(p)
        {}

        /** \brief Construct predicate and check ctor preconditions.
         */
        template<typename ... Args>
        predicate_wrapper(std::false_type, Predicate const & p, Args && ... args)
            : Predicate(p)
        {
            if(Traits::enforce_preconditions)
            {
                Expects(Predicate::operator()(args...));
            }
        }

        constexpr predicate_wrapper(predicate_wrapper &&) noexcept = default;
        constexpr predicate_wrapper(predicate_wrapper const &) noexcept = default;
        predicate_wrapper & operator=(predicate_wrapper &&) noexcept = default;
        predicate_wrapper & operator=(predicate_wrapper const &) noexcept = default;
        ~predicate_wrapper() noexcept = default;
    };

    /** \brief Stores predicate and value.
     *
     * predicate_wrapper ensures ctor preconditions, this class should check ctor postconditions
     * and dereference conditions.
     */
    class value_wrapper : protected predicate_wrapper
    {
    public :
        template<typename ... Args>
        constexpr value_wrapper(std::true_type, Predicate const & p, Args && ... args)
            : predicate_wrapper(std::true_type(), p, args...)
            , m_value(std::forward<Args>(args)...)
        {}

        template<typename ... Args>
        value_wrapper(std::false_type, Predicate const & p, Args && ... args)
            : predicate_wrapper(std::false_type(), p, args...)
            , m_value(std::forward<Args>(args)...)
        {
            if(Traits::enforce_invariants)
            {
                Ensures(Predicate::operator()(m_value));
            }
        }

        /** \brief Construct Type from tuple.
         */
        template<typename Tag, typename Tuple, std::size_t ... Index>
        constexpr value_wrapper(Tag tag, Predicate const & p, Tuple && t, std::index_sequence<Index...>)
            : value_wrapper(tag, p, std::get<Index>(std::forward<Tuple>(t))...)
        {}

        constexpr value_wrapper(value_wrapper &&) noexcept = default;
        constexpr value_wrapper(value_wrapper const &) noexcept = default;

        value_wrapper & operator=(value_wrapper &&) noexcept = default;
        value_wrapper & operator=(value_wrapper const &) noexcept = default;

        ~value_wrapper() noexcept = default;

        constexpr T const & value() const noexcept
        {
            return m_value;
        }

        constexpr Predicate const & predicate() const noexcept
        {
            return *this;
        }
    private :
        T m_value;
    };
public :
    using value_type = T;
    using predicate_type = Predicate;
    using traits_type = Traits;

    /** \brief Implicit value constructor.
     *
     * This implicit version is used when traits allow implicit unsafe construction.
     *
     * Constructs Predicate by default constructor and then uses it to check
     *  the arguments or the constructed object.
     * On default :
     * - arguments are always checked
     * - constructed object is checked unless NDEBUG is defined.
     */
    template<typename ... Args,
        typename = std::enable_if_t<
            (!detail::is_subtype<Args...>::value) &&
            (Traits::unsafe_construct >= conv_level::implicit)>
        >
    subtype(Args && ... args)
        : m_value(std::false_type(), Predicate(), std::forward<Args>(args)...)
    {}

    /** \brief Explicit value constructor.
     *
     * This explicit version is used when traits enable only explicit unsafe construction.
     */
    template<typename ... Args,
        typename = std::enable_if_t<
            (!detail::is_subtype<Args...>::value) &&
            (Traits::unsafe_construct == conv_level::enable)>,
        typename = void
        >
    explicit subtype(Args && ... args)
        : m_value(std::false_type(), Predicate(), std::forward<Args>(args)...)
    {}

    /** \brief Converting move/copy constructor.
     *
     * Converts other subtype without any checking. If predicates can be
     *  implicitly converted, then we assume that the invariants are compatible.
     *
     * Enabled for safe conversion and if Traits::unsafe_conv == implicit
     */
    template<typename Subtype,
        typename IsSafe = typename subtype_conversion<subtype, Subtype>::safe,
        typename = std::enable_if_t<
            (subtype_conversion<subtype, Subtype>::conv == conv_level::implicit)>
        >
    constexpr subtype(Subtype && x)
        : m_value(IsSafe(), detail::copy_if_possible<Predicate>(x.predicate()), x.value())//TODO : forward
    {}

    /** \brief Converting move/copy constructor.
     *
     * Explicit version, enabled for unsafe conversion if Traits::unsafe_conv == enable
     */
    template<typename Subtype,
        typename = std::enable_if_t<
            (subtype_conversion<subtype, Subtype>::conv == conv_level::enable)>
        >
    explicit subtype(Subtype && x)
        : m_value(std::false_type(), detail::copy_if_possible<Predicate>(x.predicate()), x.value())//TODO : forward
    {}

    template<typename Wrap,
        typename = std::enable_if_t<is_unsafe_forward_wrapper<Wrap>::value>,
        typename = void, typename = void>
    constexpr subtype(Wrap && wrap) noexcept
        : m_value(std::true_type(), Predicate(), std::forward<Wrap>(wrap),
                std::make_index_sequence<Wrap::tuple_size>())
    {}

    template<typename Wrap,
        typename = std::enable_if_t<is_assert_forward_wrapper<Wrap>::value>,
        typename = void, typename = void, typename = void>
    subtype(Wrap && wrap) noexcept
        : m_value(std::integral_constant<bool, NDEBUG_DEFINED>(), Predicate(), std::forward<Wrap>(wrap),
                std::make_index_sequence<Wrap::tuple_size>())
    {}

    subtype(subtype &&) noexcept = default;
    subtype(subtype const &) noexcept = default;

    subtype & operator=(subtype &&) noexcept = default;
    subtype & operator=(subtype const &) noexcept = default;

    ~subtype() noexcept = default;

    constexpr T const & value() const noexcept
    {
        return m_value.value();
    }

    constexpr Predicate const & predicate() const noexcept
    {
        return m_value.predicate();
    }

    template<typename ... Args>
    static constexpr subtype unsafe_construct(Args && ... args)
    {
        return value_wrapper(std::true_type(), Predicate(), std::forward<Args>(args)...);
    }
private :
    constexpr subtype(value_wrapper && wrap)
        : m_value(std::move(wrap)) {}

    value_wrapper m_value;
};

template<typename Dst, typename Src>
bool subtype_convertible(Src const & src)
{
    auto predicate = detail::copy_if_possible<typename Dst::predicate_type>(src.predicate());
    return predicate(src.value());
}

// -- Refinement

template<typename T, typename P>
struct implicit_subtype_traits : subtype_traits<T, P>
{
    static constexpr conv_level unsafe_construct = conv_level::implicit;
    static constexpr conv_level unsafe_convert = conv_level::implicit;

    template<typename Derived>
    struct base : subtype_traits<T, P>::template base<Derived>
    {
        operator T const &() const noexcept
        {
            return static_cast<Derived const *>(this)->value();
        }
    };
};

template<typename Type, typename Predicate>
using implicit_subtype = subtype<Type, Predicate, implicit_subtype_traits<Type, Predicate>>;

template<typename T, typename P>
struct explicit_subtype_traits : subtype_traits<T, P>
{
    static constexpr conv_level unsafe_construct = conv_level::enable;
    static constexpr conv_level unsafe_convert = conv_level::enable;

    template<typename Derived>
    struct base : subtype_traits<T, P>::template base<Derived>
    {
        explicit operator T const &() const noexcept
        {
            return static_cast<Derived const *>(this)->value();
        }
    };
};

template<typename Type, typename Predicate>
using explicit_subtype = subtype<Type, Predicate, explicit_subtype_traits<Type, Predicate>>;

// -- Pointers --

/** \brief Predicate that ensures pointer alignment and validity.
 */
template<uintptr_t Alignment = 1>
struct not_null_ptr
{
    static_assert(Alignment > 0, "");
    static_assert((Alignment&(Alignment-1)) == 0, "Alignment must be power of two");

    constexpr not_null_ptr() noexcept {}

    /** \brief Conversion from better aligned pointers is OK.
     */
    template<uintptr_t A, typename = std::enable_if_t<Alignment <= A>>
    not_null_ptr(not_null_ptr<A> const &) noexcept {}

    /** \brief We can get rid of nullptr at compile time.
     */
    bool operator()(std::nullptr_t) const = delete;

    template<typename T>
    bool operator()(T const & x) const noexcept
    {
        return (x != nullptr) && ((pun::p2u(std::addressof(*x))&(Alignment-1)) == 0);
    }
};

/** \brief Specialized traits for not_null_ptr.
 *
 * Base adds derefferencing operators and disables pointer arithmetics.
 */
template<typename T, uintptr_t Alignment>
struct subtype_traits<T, not_null_ptr<Alignment>> : subtype_traits_base
{
    template<typename Derived>
    struct base : subtype_base
    {
        using element_type = typename std::pointer_traits<T>::element_type;

        element_type operator*() const
        {
            return *static_cast<Derived const *>(this)->value();
        }

        T operator->() const
        {
            return static_cast<Derived const *>(this)->value();
        }

        // TODO : enable something for pointers to array
        Derived & operator++() = delete;
        Derived & operator--() = delete;
        Derived operator++(int) = delete;
        Derived operator--(int) = delete;
        Derived & operator+(size_t) = delete;
        Derived & operator+=(size_t) = delete;
        Derived & operator-(size_t) = delete;
        Derived & operator-=(size_t) = delete;
    };
};

template<typename T, uintptr_t Alignment>
struct subtype_traits<T[], not_null_ptr<Alignment>> : subtype_traits_base
{
    template<typename Derived>
    struct base : subtype_base
    {
        using element_type = typename std::pointer_traits<T>::element_type;

        element_type & operator[](size_t i) const
        {
            return static_cast<Derived const *>(this)->value()[i];
        }

        // TODO : enable something for pointers to array
        Derived & operator++() = delete;
        Derived & operator--() = delete;
        Derived operator++(int) = delete;
        Derived operator--(int) = delete;
        Derived & operator+(size_t) = delete;
        Derived & operator+=(size_t) = delete;
        Derived & operator-(size_t) = delete;
        Derived & operator-=(size_t) = delete;
    };
};

/** \brief Alternative to gsl::not_null using subtype.
 */
template<typename T, uintptr_t Alignment = alignof(typename std::pointer_traits<T>::element_type)>
using not_null = implicit_subtype<T, not_null_ptr<Alignment>>;

template<typename T, uintptr_t Alignment = alignof(typename std::pointer_traits<T>::element_type)>
using not_null_strict = subtype<T, not_null_ptr<Alignment>>;

}//namespace butil

#endif//BUTIL_SUBTYPE_H_INCLUDED
