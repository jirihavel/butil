#ifndef BUTIL_STRING_H_INCLUDED
#define BUTIL_STRING_H_INCLUDED

#include <string_ref.h>

#include <sstream>
#include <string>

#if HAVE_BOOST
# include <boost/lexical_cast.hpp>
#endif//HAVE_BOOST

namespace butil {

namespace detail {
    
// Convert anything to string

using std::to_string;

#if HAVE_BOOST
template<typename T>
std::string to_string(T && x)
{
    return boost::lexical_cast<std::string>(std::forward<T>(x));
}
#endif//HAVE_BOOST

// Modify argument to stringish one with O(1) length
// - char ptr is converted to string_ref
// - nonstring stuff is converted to string

constexpr inline char str_mod(char c) noexcept
{
    return c;
}

inline std::string_ref str_mod(char const * str) noexcept
{
    return str;
}

inline std::string_ref str_mod(std::string_ref str) noexcept
{
    return str;
}

inline std::string_ref str_mod(std::string const & str) noexcept
{
    return str;
}

template<typename T>
auto str_mod(std::reference_wrapper<T> ref) -> decltype(str_mod(ref.get()))
{
    return str_mod(ref.get());
}

template<typename T>
std::string str_mod(T const & x)
{
    return to_string(x);
}

}//namespace detail
/*
template<typename T>
using iterated_value = std::pair<T, size_t>;

template<typename Char, typename Traits, typename Type>
std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & o, iterated_value<Type> const & x)
{
    for(size_t i = 0; i < x.second; ++i)
        o << x.first;
    return o;
}

template<typename T>
auto val_iter(T && x, size_t n)
{
    return iterated_value<std::decay_t<T>>(std::forward<T>(x), n);
}

template<typename T>
auto ref_iter(T const & x, size_t n)
{
    return iterated_value<T const &>(x, n);
}
*/
namespace detail {
/*
template<typename T>
auto str_mod(iterated_value<T> const & x)
{
    return val_iter(str_mod(x.first), x.second);
}
*/
// Calculate total length of stringish arguments

constexpr inline size_t str_len(char) noexcept
{
    return 1;
}

inline size_t str_len(std::string const & str) noexcept
{
    return str.size();
}

inline size_t str_len(std::string_ref str) noexcept
{
    return str.size();
}
/*
template<typename T>
size_t str_len(iterated_value<T> const & x) noexcept
{
    return str_len(x.first)*x.second;
}
*/
// Variadic length

constexpr inline size_t str_cat_len() noexcept
{
    return 0;
}

template<typename T, typename ... Args>
inline size_t str_cat_len(T const & x, Args const & ... args) noexcept
{
    return str_len(x) + str_cat_len(args...);
}

// Append stringish arguments

inline void str_app(std::string & str, char c)
{
    str += c;
}

inline void str_app(std::string & str, std::string const & s)
{
    str += s;
}

inline void str_app(std::string & str, std::string_ref s)
{
    str.append(s.begin(), s.end());
}
/*
template<typename T>
void str_app(std::string & str, iterated_value<T> const & x)
{
    for(size_t i = 0; i < x.second; ++i)
        str_app(str, x.first);
}
*/
// Variadic concatenation

inline void str_cat_app(std::string &) noexcept
{
}

template<typename T, typename ... Args>
inline void str_cat_app(std::string & str, T const & x, Args const & ... args)
{
    str_app(str, x);
    str_cat_app(str, args...);
}

template<typename ... Args>
inline void str_cat_impl(std::string & str, Args const & ... args)
{
    str.reserve(str.size() + str_cat_len(args...));
    str_cat_app(str, args...);
}

}//namespace detail

template<typename ... Args>
inline std::string str_cat(Args const & ... args)
{
    std::string str;
    detail::str_cat_impl(str, detail::str_mod(args)...);
    return str;
}

template<typename ... Args>
inline std::string str_cat(std::string && s, Args const & ... args)
{
    std::string str = std::move(s);
    detail::str_cat_impl(str, detail::str_mod(args)...);
    return str;
}

}//namespace butil

#endif//BUTIL_STRING_H_INCLUDED
