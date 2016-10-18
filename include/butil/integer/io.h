#ifndef BUTIL_INTEGER_IO_H_INCLUDED
#define BUTIL_INTEGER_IO_H_INCLUDED

#include <butil/integer/intset.h>
#include <butil/integer/value.h>
#include <butil/io/debug.h>

#include <ostream>

namespace butil {
namespace integer {

template<typename Char, typename Traits, typename Set, value_flags Flags, typename Int>
std::basic_ostream<Char, Traits> & operator<<(
        std::basic_ostream<Char, Traits> & stream,
        intset_value<Set, Flags, Int> const & x)
{
    return stream << +x.value();
}

}//namespace integer
}//namespace butil

namespace std {

template<typename Char, typename Traits, typename X>
auto operator<<(basic_ostream<Char, Traits> & stream, butil::io::debug_wrapper<X> const & x)
    -> decltype(butil::integer::intset_traits<X>::value(x.get()), stream)
{
    using butil::integer::intset_traits;
    return stream << '(' << +intset_traits<X>::value(x.get()) << " in ["
        << +intset_traits<X>::min << ", " << +intset_traits<X>::max << "])";
}

}//namespace std

#endif//BUTIL_INTEGER_IO_H_INCLUDED
