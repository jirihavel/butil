#ifndef BUTIL_ARITHMETIC_INTEGRAL_CONSTANT_H_INCLUDED
#define BUTIL_ARITHMETIC_INTEGRAL_CONSTANT_H_INCLUDED
#if 0
namespace butil {

template<typename T, T X>
struct arithmetic_integral_constant
    : std::integral_constant<T, X>
{
};

template<typename T, T X>
constexpr auto operator+(arithmetic_integral_constant<T, X> x) noexcept
{
    return x;
}

namespace detail {

template<bool uint, typename T, T X>
struct aic_neg_impl
{
};

}//namespace detail
/*
template<typename T, T X>
constexpr auto operator-(arithmetic_integral_constant<T, X> x) noexcept
{
    //TODO ...
//    static_assert(INTMAX_MIN+1 == -INTMAX_MAX, "");
//    static_assert(X <= INTMAX_MAX+1, "");
    return typename std::conditional<X < 0,
           arithmetic_integral_constant<uintmax_t, >, //[INTMAX_MIN, 0]
           arithmetic_integral_constant< intmax_t, >  //[0, INTMAX_MAX+1]
       >::type();
}
*/
}//namespace butil
#endif
#endif//BUTIL_ARITHMETIC_INTEGRAL_CONSTANT_H_INCLUDED
