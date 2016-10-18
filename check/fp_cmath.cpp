#include <butil/fp/cmath.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("FP cmath", "[fp]")
{
    CHECK(fp::sin(0) == Approx(0));
    CHECK(fp::cos(0) == Approx(1));
    CHECK(fp::tan(0) == Approx(0));
}
