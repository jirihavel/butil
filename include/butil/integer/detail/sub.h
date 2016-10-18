#ifndef BUTIL_INTEGER_DETAIL_SUB_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_SUB_H_INCLUDED

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

template<typename X>
constexpr bool sub_negative_overflow(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    // a - b < min
    return (b > 0) && (a < (boost::integer_traits<X>::const_min + b));
}

template<typename X>
constexpr bool sub_positive_overflow(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    // a - b > max
    return (b < 0) && (a > (boost::integer_traits<X>::const_max + b));
}

template<typename X>
constexpr bool sub_overflow(X a, X b) noexcept
{
    return sub_negative_overflow<X>(a, b)
        || sub_positive_overflow<X>(a, b);
}

/**
 * Subtract two integers as unsigned to avoid undefined behavior of signed overflow.
 */
template<typename X>
constexpr X sub_wrap(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    using uint = std::make_unsigned_t<X>;
    return static_cast<X>(static_cast<uint>(a) - static_cast<uint>(b));
}

namespace detail {

template<bool Overflow, typename X, X A, X B>
struct integral_sub_base
    : std::integral_constant<X, A-B>
{
};

template<typename X, X A, X B>
struct integral_sub_base<true, X, A, B>
{
    using value_type = X;
};

}//namespace detail

template<typename X, X A, X B>
struct integral_sub
    : detail::integral_sub_base<sub_overflow<X>(A, B), X, A, B>
{
    static constexpr bool negative_overflow = sub_negative_overflow<X>(A, B);
    static constexpr bool positive_overflow = sub_positive_overflow<X>(A, B);
    static constexpr bool overflow = negative_overflow | positive_overflow;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_SUB_H_INCLUDED
