#ifndef BUTIL_INTEGER_MULTIPLICATION_H_INCLUDED
#define BUTIL_INTEGER_MULTIPLICATION_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/mul.h>

namespace butil {
namespace integer {

template<typename X>
constexpr auto mul(X x) noexcept
{
    return intset_value<intset_interval<X>>(x);
}

template<typename X, typename ... Args>
constexpr auto mul(X x, Args ... args) noexcept
{
    using rhs = decltype(mul(args...));
    using result = interval_mul<X, rhs>;
    using common = typename interval_type<result, X, rhs>::fast;
    return intset_value<result>(
          static_cast<common>(intset_traits<X>::value(x))
        * static_cast<common>(mul(args...).value()));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_MULTIPLICATION_H_INCLUDED
