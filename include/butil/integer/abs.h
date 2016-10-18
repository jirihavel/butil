#ifndef BUTIL_INTEGER_ABSOLUTE_VALUE_H_INCLUDED
#define BUTIL_INTEGER_ABSOLUTE_VALUE_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/abs.h>

namespace butil {
namespace integer {

template<typename X>
constexpr auto iabs(X const & x) noexcept
    -> intset_value<interval_abs<X>>
{
    return intset_value<interval_abs<X>>(uabs(intset_traits<X>::value(x)));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_ABSOLUTE_VALUE_H_INCLUDED
