#ifndef BUTIL_IO_ITEMIZE_H_INCLUDED
#define BUTIL_IO_ITEMIZE_H_INCLUDED

#include <functional>
#include <iterator>
#include <ostream>
#include <utility>

namespace butil {
namespace io {

template<typename Range, typename Bullet, typename Filler>
class itemized_range
{
    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, itemized_range const & self)
    {
        auto const ref = std::ref(self.m_range);
        using std::begin;
        using std::end;
        auto b = begin(ref.get());
        auto const e = end(ref.get());
        if(b != e)
        {
            stream << self.m_bullet << *b;
            while(++b != e)
            {
                stream << self.m_filler << self.m_bullet << *b;
            }
        }
        return stream;
    }
public :
    template<typename R, typename B, typename F>
    itemized_range(R && r, B && b, F && f)
        : m_range(std::forward<R>(r))
        , m_bullet(std::forward<B>(b))
        , m_filler(std::forward<F>(f))
    {}
private :
    Range m_range;
    Bullet m_bullet;
    Filler m_filler;
};

/** 
 * prints (endl must be part of <filler>) :
 * <bullet><elem 1><filler>
 * <bullet><elem 2><filler>
 * ...
 * <bullet><elem n>
 */
template<typename R, typename B, typename F>
auto itemize(R && r, B && b, F && f)
{
    return itemized_range<std::decay_t<R>, std::decay_t<B>, std::decay_t<F>>(std::forward<R>(r), std::forward<B>(b), std::forward<F>(f));
}

}//namespace io
}//namespace butil

#endif//BUTIL_IO_ITEMIZE_H_INCLUDED
