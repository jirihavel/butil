#ifndef BUTIL_FP_CMATH_H_INCLUDED
#define BUTIL_FP_CMATH_H_INCLUDED

#include <cmath>

namespace butil {
namespace fp {

#define DEF_UNARY(name) struct name##_t { \
    template<typename T>                  \
    auto operator()(T x) const            \
        { return ::std::name(x); } };     \
    constexpr name##_t name

DEF_UNARY(cos);
DEF_UNARY(sin);
DEF_UNARY(tan);
DEF_UNARY(acos);
DEF_UNARY(asin);
DEF_UNARY(atan);

DEF_UNARY(cosh);
DEF_UNARY(sinh);
DEF_UNARY(tanh);
DEF_UNARY(acosh);
DEF_UNARY(asinh);
DEF_UNARY(atanh);

#define DEF_BINARY(name) struct name##_t { \
    template<typename T, typename U>       \
    auto operator()(T x, U y) const        \
        { return ::std::name(x, y); } };   \
    constexpr name##_t name

DEF_BINARY(atan2);

#undef DEF_UNARY
#undef DEF_BINARY

}//namespace fp
}//namespace butil

#endif//BUTIL_FP_CMATH_H_INCLUDED
