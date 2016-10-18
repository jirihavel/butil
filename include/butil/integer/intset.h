#ifndef BUTIL_INTEGER_INTSET_H_INCLUDED
#define BUTIL_INTEGER_INTSET_H_INCLUDED

#include <butil/integer/select.h>

#include <butil/integer/detail/cmp.h>

#include <type_traits>

#include <boost/integer_traits.hpp>

/*
 * Intset is a type that represents a set of builtin integer values.
 *
 * Some intsets can contain single value or some error flags (overflow, range/domain error)
 *
 * Examples : 
 * - builtin integers
 * - integral_constant
 * - integer_sequence
 * - integer intervals
 */

namespace butil {
namespace integer {
namespace detail {

enum class basic_intset_type
{
    builtin_integer, integral_constant, generic_intset
};

template<typename T, T N>
std::integral_constant<basic_intset_type, basic_intset_type::integral_constant> classify_basic_intset(std::integral_constant<T,N>);

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
std::integral_constant<basic_intset_type, basic_intset_type::builtin_integer> classify_basic_intset(T);

template<typename T, typename = std::enable_if_t<!std::is_integral<T>::value>>
std::integral_constant<basic_intset_type, basic_intset_type::generic_intset> classify_basic_intset(T);

template<basic_intset_type Type, typename T>
struct intset_traits_impl
{
    static_assert(cmp::int_le(T::min, T::max), "");
    // TODO : deal with different types
    static_assert(std::is_same<decltype(T::min), decltype(T::max)>::value, "");
    // boundaries
    using value_type = std::decay_t<decltype(T::min)>;
    static constexpr value_type min = T::min;
    static constexpr value_type max = T::max;

    static constexpr value_type value(T const & x) noexcept
    {
        return x.value();
    }

    static constexpr T make(value_type x) noexcept
    {
        return static_cast<T>(x);
    }
};

template<typename T>
struct intset_traits_impl<basic_intset_type::builtin_integer, T>
{
    static_assert(std::is_integral<T>::value, "");
    using value_type = T;
    static constexpr value_type min = boost::integer_traits<T>::const_min;
    static constexpr value_type max = boost::integer_traits<T>::const_max;

    static constexpr T value(T x) noexcept
    {
        return x;
    }

    static constexpr T make(T x) noexcept
    {
        return x;
    }
};

template<typename T>
struct intset_traits_impl<basic_intset_type::integral_constant, T>
{
    using value_type = typename T::value_type;
    static constexpr value_type min = T::value;
    static constexpr value_type max = T::value;

    static_assert(std::is_integral<value_type>::value, "");

    static constexpr value_type value(T) noexcept
    {
        return T::value;
    }

    static constexpr T make(value_type x) noexcept
    {
        return assert(T::value == x), T();
    }
};

}//namespace detail

/** \brief Traits of an set of integers.
 *
 * value_type - boundary type + storage type if applicable
 * boundaries min and max
 *
 * Works for anything with static constant members min and max.
 * Specialized for
 * - builtin integers
 * - std::integral_constant and derived stuff
 */
template<typename T>
struct intset_traits
    : detail::intset_traits_impl<decltype(detail::classify_basic_intset(std::declval<T>()))::value, T>
{};

//------------------------------------------------------------------------------
// -- Relation operators --
//
// ordering - A < B <=> forall a in A, b in B : a < b
//
// no test for equality, since intsets can be sparse
//------------------------------------------------------------------------------

template<typename A, typename B>
using intset_less = std::integral_constant<bool,
      cmp::int_lt(intset_traits<A>::max, intset_traits<B>::min)>;

template<typename A, typename B>
using intset_greater = intset_less<B, A>;

template<typename A, typename B>
using intset_less_equal = std::integral_constant<bool,
      cmp::int_le(intset_traits<A>::max, intset_traits<B>::min)>;

template<typename A, typename B>
using intset_greater_equal = intset_less_equal<B, A>;

//------------------------------------------------------------------------------
// -- Type selection --
//------------------------------------------------------------------------------

enum prefer_type
{
    prefer_unsigned = 0,
    prefer_signed = 1,
};

namespace detail {

template<bool want_signed, typename T>
struct intset_type_impl
    : int_range_value_t<intset_traits<T>::min, intset_traits<T>::max>
{};

template<typename T>
struct intset_type_impl<false, T>
    : boost::uint_value_t<intset_traits<T>::max>
{};

}//namespace detail

template<typename T, prefer_type Pref = prefer_signed>
struct intset_type
{
    static constexpr bool want_signed = (intset_traits<T>::min < 0)
        || ((Pref == prefer_signed) && (intset_traits<T>::max <= INTMAX_MAX));
    using least = typename detail::intset_type_impl<want_signed, T>::least;
    using fast = typename boost::int_fast_t<least>::type;
    using max = int_max_t<least>;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_INTSET_H_INCLUDED
