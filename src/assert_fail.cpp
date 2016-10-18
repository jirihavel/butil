#include <butil/assert_fail.h>

#include <cstdio>
#include <cstdlib>

namespace butil {
namespace internal {
void assert_fail(char const * func, char const * file, unsigned line, char const * msg)
{
    fprintf(stderr, "%s %s:%u:%s.\n", func, file, line, msg);
    abort();
}
}//namespace internal
}//namespace butil
