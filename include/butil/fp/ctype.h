#ifndef BUTIL_FP_CTYPE_H_INCLUDED
#define BUTIL_FP_CTYPE_H_INCLUDED

#include <cctype>

namespace butil {
namespace fp {

#define DEF_PRED(name) struct name##_t {            \
    constexpr bool operator()(int c) const noexcept \
        { return ::std::name(c); } };               \
    constexpr name##_t name

DEF_PRED(isalnum);
DEF_PRED(isalpha);
DEF_PRED(isblank);
DEF_PRED(iscntrl);
DEF_PRED(isdigit);
DEF_PRED(isgraph);
DEF_PRED(islower);
DEF_PRED(isprint);
DEF_PRED(ispunct);
DEF_PRED(isspace);
DEF_PRED(isupper);
DEF_PRED(isxdigit);

#undef DEF_PRED

#define DEF_FUNC(name) struct name##_t {           \
    constexpr int operator()(int c) const noexcept \
        { return ::std::name(c); } };              \
    constexpr name##_t name

DEF_FUNC(tolower);
DEF_FUNC(toupper);

#undef DEF_FUNC

}//namespace fp
}//namespace butil

#endif//BUTIL_FP_CTYPE_H_INCLUDED
