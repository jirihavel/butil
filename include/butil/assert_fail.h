#ifndef BUTIL_ASSERT_FAIL_H_INCLUDED
#define BUTIL_ASSERT_FAIL_H_INCLUDED

#include <cassert>

namespace butil {
namespace internal {

[[noreturn]]
void assert_fail(char const * func, char const * file, unsigned line, char const * msg);

}//namespace internal
}//namespace butil

/** \brief Alternative to assert(!"Bla bla bla").
 *
 * assert(false) does generate lots of stupid warnings. This one is quiet.
 */
#define BUTIL_ASSERT_FAIL(msg) butil::internal::assert_fail(__func__, __FILE__, __LINE__, (msg))

#ifdef NDEBUG
# define BUTIL_ASSUME(cond) do { if(!(cond)) __builtin_unreachable(); } while(false)
#else
# define BUTIL_ASSUME assert
#endif

#endif//BUTIL_ASSERT_FAIL_H_INCLUDED
