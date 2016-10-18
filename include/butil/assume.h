#ifndef BUTIL_ASSUME_H_INCLUDED
#define BUTIL_ASSUME_H_INCLUDED

#include <butil/fail_fast.h>

#include <type_traits>
#include <utility>

namespace butil {

template<typename T>
class assumption
{
public :
    template<typename ... Args>
    explicit assumption(Args && ... args)
        : m_value(std::forward<Args>(args)...) {}

    template<typename X>
    decltype(auto) operator=(X && x) const
    {
        Expects(x == m_value);
        return std::forward<X>(x);
    }
private :
    T m_value;
};

template<>
class assumption<std::nullptr_t>
{
public :
    constexpr assumption() noexcept {}
    constexpr assumption(std::nullptr_t) noexcept {}

    template<typename X>
    decltype(auto) operator=(X && x) const
    {
        Expects(x == nullptr);
        return std::forward<X>(x);
    }
};

template<typename T, T N>
class assumption<std::integral_constant<T, N>>
{
public :
    constexpr assumption() noexcept {}
    constexpr assumption(std::integral_constant<T, N>) noexcept {}

    template<typename X>
    decltype(auto) operator=(X && x) const
    {
        Expects(x == N);
        return std::forward<X>(x);
    }
};

template<typename T>
inline auto assume(T && x)
{
    return assumption<std::decay_t<T>>(std::forward<T>(x));
}

using assume_null_t = assumption<std::nullptr_t>;
using assume_true_t = assumption<std::true_type>;
using assume_false_t = assumption<std::false_type>;

constexpr assume_null_t assume_null;
constexpr assume_true_t assume_true;
constexpr assume_false_t assume_false;

}//namespace butil

#endif//BUTIL_ASSUME_H_INCLUDED
