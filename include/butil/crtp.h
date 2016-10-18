#ifndef BUTIL_CRTP_H_INCLUDED
#define BUTIL_CRTP_H_INCLUDED

#include <type_traits>

namespace butil {

template<typename Derived>
struct crtp_base
{
    static_assert(std::is_base_of<crtp_base, Derived>::value, "");

    Derived & derived()
        { return *static_cast<Derived*>(this); }
    Derived const & derived() const
        { return *static_cast<Derived const*>(this); }
};

}//namespace butil

#endif//BUTIL_CRTP_H_INCLUDED
