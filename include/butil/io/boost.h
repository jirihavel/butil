#ifndef BUTIL_IO_BOOST_H_INCLUDED
#define BUTIL_IO_BOOST_H_INCLUDED

#include <boost/io/ios_state.hpp>

namespace butil {
namespace io {

using namespace boost::io;

template<typename Type, std::ios_base::fmtflags Flags, std::ios_base::fmtflags Mask>
class value_with_flags
{
    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, value_with_flags const & self)
    {
        boost::io::ios_flags_saver ifs(stream);
        stream.setf(Flags, Mask);
        return stream << self.m_value;
    }
public :
    template<typename T,
        typename = std::enable_if_t<std::is_constructible<Type, T&&>::value>>
    value_with_flags(T && x) : m_value(std::forward<T>(x)) {}
private :
    Type m_value;
};

template<std::ios_base::fmtflags Flags, std::ios_base::fmtflags Mask = Flags, typename T>
auto with_(T && x)
{
    return value_with_flags<std::decay_t<T>, Flags, Mask>(std::forward<T>(x));
}

template<typename T>
auto hex_base(T && x)
{
    return with_<std::ios_base::hex|std::ios_base::showbase, std::ios_base::basefield|std::ios_base::showbase>(std::forward<T>(x));
}

}//namespace io
}//namespace butil

#endif//BUTIL_IO_BOOST_H_INCLUDED
