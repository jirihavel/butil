#ifndef BUTIL_TYPE_PUN_H_INCLUDED
#define BUTIL_TYPE_PUN_H_INCLUDED

#include <cstdint>

namespace butil {

/** \brief Type puning - unsafe casts around type system.
 */
namespace pun {

template<typename T>
union ptr_punion
{
    T * p;
    std::intptr_t i;
    std::uintptr_t u;
};

/** \brief Pointer to signed integer.
 */
template<typename T>
inline std::intptr_t p2i(T * x) noexcept
{
    return ptr_punion<T>{.p = x}.i;
}

/** \brief Pointer to unsigned integer.
 */
template<typename T>
inline std::uintptr_t p2u(T * x) noexcept
{
    return ptr_punion<T>{.p = x}.u;
}

/** \brief Signed integer to pointer.
 */
template<typename T>
inline T * i2p(std::intptr_t x) noexcept
{
    return ptr_punion<T>{.i = x}.p;
}

/** \brief Unsigned integer to pointer.
 */
template<typename T>
inline T * u2p(std::uintptr_t x) noexcept
{
    return ptr_punion<T>{.u = x}.p;
}

union flt_punion
{
    float f;
    std::int32_t i;
    std::uint32_t u;
};

inline std::int32_t f2i(float x) noexcept
{
    return flt_punion{.f = x}.i;
}

inline std::uint32_t f2u(float x) noexcept
{
    return flt_punion{.f = x}.u;
}

inline float i2f(std::int32_t x) noexcept
{
    return flt_punion{.i = x}.f;
}

inline float u2f(std::uint32_t x) noexcept
{
    return flt_punion{.u = x}.f;
}

/** \brief Ordered conversion of float to uint.
 *
 * a < b -> f2u_ord(a) < f2u_ord(b)
 */
inline uint32_t f2u_ord(float f) noexcept
{
    uint32_t const u = f2u(f);
	uint32_t const mask =
        -static_cast<int32_t>(u >> 31) | UINT32_C(0x80000000);
	return u ^ mask;
}

}//namespace pun
}//namespace butil

#endif//BUTIL_TYPE_PUN_H_INCLUDED
