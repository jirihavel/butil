#ifndef BUTIL_IO_INTERSPERSE_H_INCLUDED
#define BUTIL_IO_INTERSPERSE_H_INCLUDED

#include <functional>
#include <iterator>
#include <ostream>
#include <utility>

namespace butil {
namespace io {

template<typename Range, typename Filler>
class interspersed_range
    : private std::pair<Range, Filler>
{
    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, interspersed_range const & self)
    {
        auto const ref = std::ref(self.first);//if range is already ref, it will collapse
        using std::begin;
        using std::end;
        auto b = begin(ref.get());
        auto const e = end(ref.get());
        if(b != e)
        {
            stream << *b;
            while(++b != e)
            {
                stream << self.second << *b;
            }
        }
        return stream;
    }
public :
    template<typename R, typename F>
    interspersed_range(R && r, F && f)
        : std::pair<Range, Filler>(std::forward<R>(r), std::forward<F>(f))
    {}
};

template<typename R, typename F>
auto intersperse(R && r, F && f)
{
    return interspersed_range<std::decay_t<R>, std::decay_t<F>>(std::forward<R>(r), std::forward<F>(f));
}

}//namespace io
}//namespace butil

#endif//BUTIL_IO_INTERSPERSE_H_INCLUDED
