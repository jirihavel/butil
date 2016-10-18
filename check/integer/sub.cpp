#include <catch.hpp>

#include <butil/integer/sub.h>

using namespace butil::integer;

TEST_CASE("Subtraction overflow test functions", "[integer]")
{
    SECTION("signed")
    {
        for(int16_t i = INT8_MIN; i <= INT8_MAX; ++i)
        {
            for(int16_t j = INT8_MIN; j <= INT8_MAX; ++j)
            {
                CAPTURE(i);
                CAPTURE(j);
                int16_t const y = i-j;
                CHECK(sub_negative_overflow<int8_t>(i, j) == (y < INT8_MIN));
                CHECK(sub_positive_overflow<int8_t>(i, j) == (y > INT8_MAX));
                CHECK(sub_overflow<int8_t>(i, j) == ((y < INT8_MIN) || (y > INT8_MAX)));
            }
        }
    }

    SECTION("unsigned")
    {
        for(int16_t i = 0; i <= UINT8_MAX; ++i)
        {
            for(int16_t j = 0; j <= UINT8_MAX; ++j)
            {
                CAPTURE(i);
                CAPTURE(j);
                int16_t const y = i-j;
                CHECK(sub_negative_overflow<uint8_t>(i, j) == (y < 0));
                CHECK(!sub_positive_overflow<uint8_t>(i, j));
                CHECK(sub_overflow<uint8_t>(i, j) == (y < 0));
            }
        }
    }
}

TEST_CASE("sub", "[integer]")
{
    auto y = sub(INT_MAX, -INT_MAX);
    CHECK(y.flags() == value_flags::none);
    CHECK(y.value() == (intmax_t(INT_MAX)+INT_MAX));
}
