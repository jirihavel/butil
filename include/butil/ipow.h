#ifndef BUTIL_INTEGRAL_POWER_H_INCLUDED
#define BUTIL_INTEGRAL_POWER_H_INCLUDED

#include <butil/inv.h>

#include <cstdint>
#include <climits>

#include <ratio>

namespace butil {

template<typename T>
T upow(T base, unsigned exp) noexcept
{
    T result = 1;
    while(exp)
    {
        if((exp&1) != 0)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

namespace detail {

template<uintmax_t N, uintmax_t D>
struct ipow_impl
{
    template<typename T>
    auto operator()(T const & x) const noexcept
    {
        return std::pow(x, static_cast<corresponding_float<T>>(N)/D);
    }
};

template<uintmax_t N>
struct ipow_impl<N,2>
{
    template<typename T>
    auto operator()(T const & x) const noexcept
    {
        return std::sqrt(ipow_impl<N,1>()(x));
    }
};

template<uintmax_t N>
struct ipow_impl<N,3>
{
    template<typename T>
    auto operator()(T const & x) const noexcept
    {
        return std::cbrt(ipow_impl<N,1>()(x));
    }
};

template<uintmax_t N>
struct ipow_impl<N,1>
{
    template<typename T>
    auto operator()(T const & x) const noexcept
    {
        auto const aux = ipow_impl<N/2,1>()(x);
        return N%2 ? aux*aux*x : aux*aux;
    }
};

template<>
struct ipow_impl<0,1>
{
    template<typename T>
    auto operator()(T const &) const noexcept
    {
        return 1;
    }
};

template<bool I>
struct opt_inv
{
    template<typename T>
    T operator()(T const & x) const noexcept
    {
        return x;
    }
};

template<>
struct opt_inv<true>
{
    template<typename T>
    auto operator()(T const & x) const noexcept
    {
        return inv(x);
    }
};

}//namespace detail

/** \brief Power to the compile time rational number.
 *
 * \returns x^(N/D)
 *
 * Return type is T if D==1 and N >= 0. Otherwise float or [long] double is returned.
 */
template<intmax_t N, intmax_t D = 1, typename T>
auto ipow_(T const & x) noexcept
{
    using r = std::ratio<N,D>;//use std::ratio for normalization
    static_assert(r::num > INTMAX_MIN, "Absolute value is undefined");
    static_assert(r::den > 0, "Normalization failure");
    return detail::opt_inv<(r::num<0)>()(
        detail::ipow_impl<(r::num<0?-r::num:r::num),r::den>()(x));
}

template<typename T>
auto sqr(T const & x) noexcept
{
    return ipow_<2>(x);
}

}//namespace butil

#endif//BUTIL_INTEGRAL_POWER_H_INCLUDED
