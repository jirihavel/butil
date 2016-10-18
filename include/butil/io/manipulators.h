#ifndef BUTIL_IO_MANIPULATORS_H_INCLUDED
#define BUTIL_IO_MANIPULATORS_H_INCLUDED

#include <ostream>

namespace std {

/** Allows any functor to be ostream manipulator.
 *
 * In std, any function ostream & -> ostream & can be used as ostream manipulator.
 * This operator allows any functor with proper operator() to be used too.
 */
template<typename Char, typename Traits, typename F>
auto operator<<(std::basic_ostream<Char, Traits> & o, F const & f) -> decltype(f(o),o)
{
    return f(o);
}

}//namespace std

#endif//BUTIL_IO_MANIPULATORS_H_INCLUDED
