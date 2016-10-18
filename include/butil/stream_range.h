#ifndef BUTIL_STREAM_RANGE_H_INCLUDED
#define BUTIL_STREAM_RANGE_H_INCLUDED

#include <iterator>
#include <ostream>

namespace butil {

template<typename Char, typename Traits, typename Range, typename Delimiter>
std::basic_ostream<Char, Traits> & write_range(std::basic_ostream<Char, Traits> & stream, Range const & range, Delimiter const & delimiter)
{
    using std::begin;
    auto b = begin(range);
    using std::end;
    auto const e = end(range);
    if(b != e)
    {
        stream << *b;
        while(++b != e)
        {
            stream << delimiter << *b;
        }
    }
    return stream;
}

}//namespace butil

#endif//BUTIL_STREAM_RANGE_H_INCLUDED
