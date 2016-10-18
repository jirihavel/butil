#ifndef BUTIL_INVERSE_H_INCLUDED
#define BUTIL_INVERSE_H_INCLUDED

#include <cmath>

#include <type_traits>

namespace butil {

/** \defgroup InverseUtil Multiplicative inverse similar to cmath functions like abs or sqrt.
 * @{
 */
    
inline constexpr float       inv(float       x) noexcept { return 1/x; }
inline constexpr double      inv(double      x) noexcept { return 1/x; }
inline constexpr long double inv(long double x) noexcept { return 1/x; }

template<typename T>
constexpr std::enable_if_t<std::is_integral<T>::value,double> inv(T x) noexcept
{
    // all integral types are converted to double
    return inv(static_cast<double>(x));
}

/** @} */

namespace detail {

template<typename T>
using corresponding_float = decltype(inv(T()));

}//detail

}//namespace butil

#endif//BUTIL_INVERSE_H_INCLUDED
