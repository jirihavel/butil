#include <butil/subtype.h>
#include <butil/fp/constant.h>

#include <catch.hpp>

using namespace butil;

struct TagA : fp::constant_true_t {};
struct TagB : fp::constant_true_t {};

using IntA = explicit_subtype<int, TagA>;
using IntB = explicit_subtype<int, TagB>;

int foo(IntA x)
{
    return x.value();
}

int foo(IntB x)
{
    return -x.value();
}

TEST_CASE("Subtype tagging", "[subtype]")
{
    REQUIRE(foo(IntA(1)) > 0);
    REQUIRE(foo(IntB(1)) < 0);
}
