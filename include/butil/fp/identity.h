#ifndef BUTIL_FP_IDENTITY_H_INCLUDED
#define BUTIL_FP_IDENTITY_H_INCLUDED

#include <utility>

namespace butil {
namespace fp {

struct identity_t
{
    template<typename X>
    constexpr decltype(auto) operator()(X && x) const
    {
        return std::forward<X>(x);
    }
};

constexpr identity_t identity;

}//namespace fp
}//namespace butil

#endif//BUTIL_FP_IDENTITY_H_INCLUDED
