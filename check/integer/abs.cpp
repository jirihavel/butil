#include <catch.hpp>

#include <butil/integer/abs.h>

using namespace butil::integer;

TEST_CASE("abs signed overflow", "[integer]")
{
    CHECK(abs_overflow(INT_MIN));

    // !!! INT8_MAX can be int instead of int8_t :)
    CHECK(abs_overflow<int8_t>(INT8_MIN));
    CHECK(abs_overflow<int16_t>(INT16_MIN));
    CHECK(abs_overflow(INT32_MIN));
    CHECK(abs_overflow(INT64_MIN));

    CHECK(uabs<int8_t>(INT8_MIN) == INT8_MAX+1);

    CHECK(uabs(-1) == 1);
    CHECK(iabs(-1).value() == 1);

    CHECK(cmp::eq(iabs<int8_t>(-1).min, 0));
    CHECK(cmp::eq(iabs<int16_t>(-1).min, 0));
    CHECK(cmp::eq(iabs<int32_t>(-1).min, 0));
    CHECK(cmp::eq(iabs<int64_t>(-1).min, 0));
}
#if 0
TEST_CASE("abs nonnegative", "[integer]")
{
/*    
    for(int16_t i = INT8_MIN+1; i <= INT8_MAX; ++i)
    {
        CHECK(!abs_overflow<int8_t>(i));
        auto const a = sabs<int8_t>(i);
        CHECK(a >= 0);
        CHECK(uint8_t(a) == uabs<int8_t>(i));
    }*/
}
#endif
