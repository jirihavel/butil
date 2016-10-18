#ifndef BUTIL_INTEGER_DETAIL_SATURATE_CAST_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_SATURATE_CAST_H_INCLUDED

#include <butil/integer/detail/cmp.h>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

template<typename D,
    D Min  = boost::integer_traits<D>::const_min,
    D Max  = boost::integer_traits<D>::const_max,
    typename S>
constexpr D int_saturate_cast(S x) noexcept
{
    static_assert(Min <= Max, "Invalid boundaries");
    return cmp::int_lt(x, Min) ? Min : cmp::int_lt(Max,x) ? Max : static_cast<D>(x);
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_SATURATE_CAST_H_INCLUDED
