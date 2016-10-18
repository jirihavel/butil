#ifndef BUTIL_ENUM_FLAGS_H_INCLUDED
#define BUTIL_ENUM_FLAGS_H_INCLUDED

#include <butil/underlying_value.h>

/** \brief Generage operators for an enum.
 *
 * C++ 11 enum class provides type safety, but makes harder to use enums as flags.
 */
#define BUTIL_ENUM_FLAGS(T) \
inline constexpr bool operator!(T x) noexcept { return butil::underlying_value(x) == 0; }; \
inline constexpr T operator~(T x) noexcept { return static_cast<T>(~butil::underlying_value(x)); }; \
inline constexpr T operator&(T x, T y) noexcept { return static_cast<T>(butil::underlying_value(x) & butil::underlying_value(y)); }; \
inline constexpr T operator|(T x, T y) noexcept { return static_cast<T>(butil::underlying_value(x) | butil::underlying_value(y)); }; \
inline constexpr T operator^(T x, T y) noexcept { return static_cast<T>(butil::underlying_value(x) ^ butil::underlying_value(y)); }; \
inline T & operator&=(T & x, T y) noexcept { x = x & y; return x; }; \
inline T & operator|=(T & x, T y) noexcept { x = x | y; return x; }; \
inline T & operator^=(T & x, T y) noexcept { x = x ^ y; return x; };

#endif//BUTIL_ENUM_FLAGS_H_INCLUDED
