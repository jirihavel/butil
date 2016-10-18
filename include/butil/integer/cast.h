#ifndef BUTIL_INTEGER_CAST_H_INCLUDED
#define BUTIL_INTEGER_CAST_H_INCLUDED

#include <butil/integer/cmp.h>
#include <butil/integer/intset.h>
#include <butil/integer/interval.h>

#include <butil/integer/detail/saturate_cast.h>
#include <butil/integer/detail/signed_cast.h>
#include <butil/integer/detail/unsigned_cast.h>

#include <cassert>

#include <stdexcept>

namespace butil {
namespace integer {
/*
// [un]signed_cast is imported from sign_cast.h

template<typename D,
    D Min  = boost::integer_traits<D>::const_min,
    D Max  = boost::integer_traits<D>::const_max,
    typename S>
constexpr D saturate_cast(S x) noexcept
{
    static_assert(Min <= Max, "Invalid boundaries");
    return cmp::lt(x,Min) ? Min : cmp::lt(Max,x) ? Max : static_cast<D>(x);
}
*/

template<typename D, typename S>
constexpr D unsafe_cast(S const & x) noexcept
{
    return intset_traits<D>::make(intset_traits<S>::value(x));
}

template<typename D, typename S>
constexpr D assert_cast(S const & x) noexcept
{
    return assert(interval_contains<D>(x)), unsafe_cast<D>(x);
}

template<typename D, typename S>
constexpr D throw_cast(S const & x) noexcept
{
    return interval_contains<D>(x) ? unsafe_cast<D>(x)
        : throw std::range_error("throw_cast failed");
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_CAST_H_INCLUDED
