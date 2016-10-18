#ifndef BUTIL_FAIL_FAST_H_INCLUDED
#define BUTIL_FAIL_FAST_H_INCLUDED

#include <cassert>

#ifdef NDEBUG
# define NDEBUG_DEFINED 1
#else
# define NDEBUG_DEFINED 0
#endif

#define BUTIL_STRINGIFY_DETAIL(x) #x
#define BUTIL_STRINGIFY(x) BUTIL_STRINGIFY_DETAIL(x)

//
// -- GSL.assert: assertions
//

#define Expects(cond) (void)(!!(cond) || (::butil::fail_fast("Precondition '" #cond "' failure at " __FILE__ " " BUTIL_STRINGIFY(__LINE__) "\n"),0))
#define Ensures(cond) (void)(!!(cond) || (::butil::fail_fast("Postcondition '" #cond "' failure at " __FILE__ " " BUTIL_STRINGIFY(__LINE__) "\n"),0))

// BUTIL_FAIL_FAST_TRAPS - debuger should react
// BUTIL_FAIL_FAST_TERMINATES - should produce core dump
// BUTIL_FAIL_FAST_THROWS - for unit tests and similar stuff
// BUTIL_FAIL_FAST_CONTINUES
//
#if !defined(BUTIL_FAIL_FAST_TRAPS) && !defined(BUTIL_FAIL_FAST_TERMINATES) && !defined(BUTIL_FAIL_FAST_THROWS) && !defined(BUTIL_FAIL_FAST_CONTINUES)
#define BUTIL_FAIL_FAST_TERMINATES 
#endif

// BUTIL_FAIL_FAST_PRINTS
//
#ifndef BUTIL_FAIL_FAST_PRINTS
# define BUTIL_FAIL_FAST_PRINTS 1
#endif

// -- Basic behavior --

#if defined(BUTIL_FAIL_FAST_TRAPS)

namespace butil {

inline void fail_fast_impl(char const * const)
{
#ifdef _MSC_VER
    __debugbreak();
#else
    __builtin_trap();
#endif
}

}//namespace butil

#elif defined(BUTIL_FAIL_FAST_TERMINATES)

#include <exception>

namespace butil {

inline void fail_fast_impl(char const * const)
{
    std::terminate();
}

}//namespace butil

#elif defined(BUTIL_FAIL_FAST_THROWS)

#include <stdexcept>

namespace butil {

struct fail_fast : std::runtime_error 
{
	explicit fail_fast(char const * const message)
        : std::runtime_error(message)
    {}
};

inline void fail_fast_impl(char const * const message)
{
    throw fail_fast(message);
}

}//namespace butil

#elif defined(BUTIL_FAIL_FAST_CONTINUES)

namespace butil {

inline void fail_fast_impl(char const * const)
{
}

}//namespace butil

#endif

// -- Printing --

#if BUTIL_FAIL_FAST_PRINTS
# include <cstdio>
#endif

namespace butil {

inline void fail_fast(char const * const message)
{
#if BUTIL_FAIL_FAST_PRINTS
    fputs(message, stderr);
#endif
    fail_fast_impl(message);
}

}//namespace butil

#endif//BUTIL_FAIL_FAST_H_INCLUDED
