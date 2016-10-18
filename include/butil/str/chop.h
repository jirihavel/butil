#ifndef BUTIL_STR_CHOP_H_INCLUDED
#define BUTIL_STR_CHOP_H_INCLUDED

#include <string_ref.h>

namespace butil {
namespace str {

template<typename Char, typename Traits, typename Predicate>
std::basic_string_ref<Char, Traits> chop(std::basic_string_ref<Char, Traits> str, Predicate pred)
{
    size_t size = str.size();
    while((size > 0) && pred(str[size-1]))
        size -= 1;
    return std::basic_string_ref<Char, Traits>(str.data(), size);
}

}//namespace str
}//namespace butil

#endif//BUTIL_STR_CHOP_H_INCLUDED
