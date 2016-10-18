#ifndef BUTIL_STR_LINES_H_INCLUDED
#define BUTIL_STR_LINES_H_INCLUDED

#include <string_ref.h>

namespace butil {
namespace str {

template<typename Char, typename Traits, typename Function>
void for_lines(std::basic_string_ref<Char, Traits> str, Function func)
{
    size_t b = 0;
    for(size_t i = 0; i < str.size(); ++i)
    {
        if(((str[i] >= 0x0A) && (str[i] <= 0x0D)) // LF, VT, FF, CR
            || (str[i] == 0x85) // NEL
            || (str[i] == 0x2028) // LS
            || (str[i] == 0x2029)) // PS
        {
            func(str.substr(b, i-b));
            b = i+1;
            // handle CR+LF
            if((str[i] == 0x0D) && (b < str.size()) && (str[b] == 0x0A))
            {
                ++i;
                ++b;
            }
        }
    }
}

}//namespace str
}//namespace butil

#endif//BUTIL_STR_LINES_H_INCLUDED
