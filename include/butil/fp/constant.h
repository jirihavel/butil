#ifndef BUTIL_FP_CONSTANT_H_INCLUDED
#define BUTIL_FP_CONSTANT_H_INCLUDED

#include <type_traits>
#include <utility>

namespace butil {
namespace fp {

template<typename Type>
class constant_t
{
public :
    template<typename ... Args>
    explicit constexpr constant_t(Args && ... args)
        : m_value(std::forward<Args>(args)...)
    {}

    constant_t(constant_t &&) = default;
    constant_t(constant_t const &) = default;
    constant_t & operator=(constant_t &&) = default;
    constant_t & operator=(constant_t const &) = default;

    template<typename ... Args>
    constexpr Type const & operator()(Args && ...) const
    {
        return m_value;
    }

    constexpr Type const & value() const
    {
        return m_value;
    }
private :
    Type const m_value;
};

template<>
struct constant_t<std::nullptr_t>
{
    template<typename ... Args>
    constexpr std::nullptr_t operator()(Args && ...) const noexcept
    {
        return nullptr;
    }

    constexpr std::nullptr_t value() const noexcept
    {
        return nullptr;
    }
};

template<typename T, T v>
struct constant_t<std::integral_constant<T, v>>
    : public std::integral_constant<T, v>
{
    template<typename ... Args>
    constexpr T operator()(Args && ...) const noexcept
    {
        return v;
    }

    constexpr T value() const noexcept
    {
        return v;
    }
};

template<typename T>
inline auto constant(T && x)
{
    return constant_t<typename std::remove_reference<T>::type>(std::forward<T>(x));
}

using constant_null_t = constant_t<std::nullptr_t>;
using constant_true_t  = constant_t<std::true_type >;
using constant_false_t = constant_t<std::false_type>;

constexpr constant_null_t constant_null;
constexpr constant_true_t constant_true;
constexpr constant_false_t constant_false;

}//namespace fp
}//namespace butil

#endif//BUTIL_FP_CONSTANT_H_INCLUDED
