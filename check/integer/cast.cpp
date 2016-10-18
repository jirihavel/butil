#include <catch.hpp>

#include <butil/integer/cast.h>

using namespace butil::integer;

TEST_CASE("saturate_cast", "[integer]")
{
    CHECK(int_saturate_cast<uint8_t>(-1) == 0);
    CHECK(int_saturate_cast<uint8_t>(INT_MAX) == UINT8_MAX);
    CHECK((int_saturate_cast<uint8_t,1,2>(-1)) == 1);
    CHECK((int_saturate_cast<uint8_t,1,2>(UINT8_MAX)) == 2);
}
