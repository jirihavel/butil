#ifndef BUTIL_INTEGER_DETAIL_DEFAULT_TYPE_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_DEFAULT_TYPE_H_INCLUDED

namespace butil {
namespace integer {

struct default_type {};

struct default_signed   : default_type {};
struct default_unsigned : default_type {};

struct default_fast  : default_type {};
struct default_least : default_type {};

struct default_fast_signed    : default_fast , default_signed   {};
struct default_fast_unsigned  : default_fast , default_unsigned {};
struct default_least_signed   : default_least, default_signed   {};
struct default_least_unsigned : default_least, default_unsigned {};

namespace detail {

template<typename Type, typename Dflt>
struct type_or_default
{
    using type = Type;
};

template<typename Dflt>
struct type_or_default<default_type, Dflt>
{
    using type = Dflt;
};

template<typename Type, typename Dflt>
using type_or_default_t = typename type_or_default<Type, Dflt>::type;

}//namespace detail

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_DEFAULT_TYPE_H_INCLUDED
