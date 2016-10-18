#ifndef BUTIL_INTEGER_LOG_H_INCLUDED
#define BUTIL_INTEGER_LOG_H_INCLUDED

#include <butil/integer/intset.h>
#include <butil/integer/cmp.h>
#include <butil/integer/detail/mul.h>

namespace butil {
namespace integer {

namespace detail {

// !!! __builtin_clz is undefined for 0

constexpr unsigned floor_log2(unsigned x)
{
    return assert(x > 0),
       sizeof(unsigned)*CHAR_BIT - __builtin_clz(x) - 1;
}

constexpr unsigned floor_log2(unsigned long x)
{
    return assert(x > 0),
       sizeof(unsigned long)*CHAR_BIT - __builtin_clzl(x) - 1;
}

constexpr unsigned floor_log2(unsigned long long x)
{
    return assert(x > 0),
       sizeof(unsigned long long)*CHAR_BIT - __builtin_clzll(x) - 1;
}

}//namespace detail

template<typename T>
constexpr auto floor_log2(T const & x)
{
    return detail::floor_log2(unsigned_cast(intset_traits<T>::value(x)));
}

template<typename T>
auto floor_log10(T const & x)
{
    assert(cmp::gt(x, 0));
    using type = typename intset_traits<T>::value_type;
    type y = 0;
    type m = 10;// next value to test
    while(cmp::lt(m, x))
    {
        y += 1;
        if(mul_overflow<T>(m, 10))
            break;
        m *= 10;
    }
    return unsigned_cast(y);
}

/*
template<uintmax_t B, typename T>
auto log_(T x)
{
    assert(x > 0);
    using type = typename intset_traits<T>::value_type;
    type y = 0;
    type m = 10;// next value to test
    while(cmp::lt(m, x))
    {
        y += 1;
        if(mul_overflow<T>(m, 10))
            break;
        m *= 10;
    }
    return y;
}
*/
}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_LOG_H_INCLUDED
