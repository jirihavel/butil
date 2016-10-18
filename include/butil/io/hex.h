#ifndef BUTIL_IO_HEX_H_INCLUDED
#define BUTIL_IO_HEX_H_INCLUDED

#include <ostream>
#include <type_traits>
#include <utility>

#include <boost/io/ios_state.hpp>

namespace butil {
namespace io {

template<typename Type>
class hexadecimal_value
{
    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, hexadecimal_value const & self)
    {
        boost::io::ios_flags_saver ifs(stream);
        return stream << std::showbase << std::hex << self.m_value;
    }
public :
    template<typename T,
        typename = std::enable_if_t<std::is_constructible<Type, T&&>::value>>
    hexadecimal_value(T && x) : m_value(std::forward<T>(x)) {}
private :
    Type m_value;
};

template<typename T>
auto hex(T && x)
{
    return hexadecimal_value<std::decay_t<T>>(std::forward<T>(x));
}

}//namespace io
}//namespace butil

#endif//BUTIL_IO_HEX_H_INCLUDED
