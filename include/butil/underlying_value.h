#ifndef BUTIL_UNDERLYING_VALUE_H
#define BUTIL_UNDERLYING_VALUE_H

#include <type_traits>

namespace butil {

/** \brief Casts enum to its underlying_type.
 */
template<typename T>
inline constexpr typename std::underlying_type<T>::type
    underlying_value(T x) noexcept
{
    return static_cast<typename std::underlying_type<T>::type>(x);
}

}//namespace butil

#endif//BUTIL_UNDERLYING_VALUE_H
