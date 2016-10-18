#ifndef BUTIL_INTEGRAL_CONSTANT_H_INCLUDED
#define BUTIL_INTEGRAL_CONSTANT_H_INCLUDED

#include <butil/integer/detail/add.h>

#include <type_traits>

namespace butil {
namespace integer {

namespace literals {

namespace detail {

constexpr inline uintmax_t parse(uintmax_t y) noexcept
{
    return y;
}

template<typename ... Chars>
constexpr uintmax_t parse(uintmax_t y, char c, Chars ... chars)
{
    return (c >= '0' && c <= '9')
        ? parse(y*10 + (c - '0'), chars...)
        : throw std::out_of_range("only decimal digits are allowed");
}

}//namespace detail

template<char ... Digits>
constexpr auto operator "" _c()
    -> std::integral_constant<uintmax_t, detail::parse(0, Digits...)>
{
    return {};
}

}//namespace literals

namespace integral_constant_arithmetics {

//template<typename A, T X, typename U, U ... Ints>
//constexpr auto cat(std::integral_constant<T, X> x, std::integer_sequence<U, Ints...> ints) noexcept
//    -> std::integer_sequence<std::common_type_t<T, U>, X, Ints...>
//{
//    return {};
//}

}//namespace integral_constant_arithmetics

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGRAL_CONSTANT_H_INCLUDED
