#ifndef BUTIL_IO_DEBUG_H_INCLUDED
#define BUTIL_IO_DEBUG_H_INCLUDED

#include <functional>
#include <ostream>

#include <boost/core/typeinfo.hpp>

namespace butil {
namespace io {

template<typename T>
struct debug_wrapper : std::reference_wrapper<T const>
{
    debug_wrapper(T const & x) : std::reference_wrapper<T const>(x) {};
};

template<typename T>
debug_wrapper<T> dbg(T const & x)
{
    return debug_wrapper<T>(x);
}

//template<typename Char, typename Traits, typename Type>
//std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, debug_wrapper<Type> const & self)
//{
//    return stream << '!' << boost::core::demangled_name(BOOST_CORE_TYPEID(Type)) << ' ' << self.get();
//}
/*
template<typename ... Types>
struct concatenated_tuple : std::tuple<Types...>
{
    template<typename ... Args>
    explicit concatenated_tuple(std::tuple<Args...> && t)
        : std::tuple<Types...>(std::move(t)) {}
};

template<typename ... Args>
auto cat(Args && ... args)
{
    return debug_wrapper<T>(x);
}

template<typename Char, typename Traits, typename Type>
std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, debug_wrapper<Type> const & self)
{
    return stream << '!' << boost::core::demangled_name(BOOST_CORE_TYPEID(Type)) << ' ' << self.get();
}
*/
}//namespace io
}//namespace butil

#endif//BUTIL_IO_DEBUG_H_INCLUDED
