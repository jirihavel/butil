#ifndef BUTIL_IO_ITERATION_H_INCLUDED
#define BUTIL_IO_ITERATION_H_INCLUDED

#include <ostream>
#include <utility>

namespace butil {
namespace io {

template<typename Type>
class iterated_value
{
    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, iterated_value const & self)
    {
        for(size_t i = 0; i < self.m_count; ++i)
            stream << self.m_value;
        return stream;
    }
public :
    template<typename T,
        typename = std::enable_if_t<std::is_constructible<Type, T&&>::value>>
    explicit iterated_value(T && x, size_t n)
        : m_value(std::forward<T>(x)), m_count(n)
    {}
private :
    Type m_value;
    size_t m_count;
};

template<typename T>
auto iterate(T && x, size_t n)
{
    return iterated_value<std::decay_t<T>>(std::forward<T>(x), n);
}

template<typename T>
auto iterate_ref(T const & x, size_t n)
{
    return iterated_value<T const &>(x, n);
}

}//namespace io
}//namespace butil

#endif//BUTIL_IO_ITERATION_H_INCLUDED
