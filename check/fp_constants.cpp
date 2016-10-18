#include <butil/fp/constant.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("FP constants", "[fp]")
{
    using namespace fp;

    CHECK(!constant_null());
    CHECK(constant_true() == true);
    CHECK(constant_false() == false);

    CHECK(!constant_null(123));
    CHECK(constant_true(123) == true);
    CHECK(constant_false(123) == false);

    CHECK(constant(0)() == 0);
    CHECK(constant(1)() == 1);
}
