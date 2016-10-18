#include <catch.hpp>

#include <butil/integer/neg.h>

using namespace butil::integer;

TEST_CASE("Negation overflow test functions", "[integer]") 
{
    SECTION("signed")
    {
        for(int16_t i = INT8_MIN; i <= INT8_MAX; ++i)
        {
            int16_t y = -i;
            CHECK(neg_negative_overflow<int8_t>(i) == (y < INT8_MIN));
            CHECK(neg_positive_overflow<int8_t>(i) == (y > INT8_MAX));
            CHECK(neg_overflow<int8_t>(i) == ((y < INT8_MIN) || (y > INT8_MAX)));
        }
    }

    SECTION("unsigned")
    {
        for(int16_t i = 0; i <= UINT8_MAX; ++i)
        {
            int16_t y = -i;
            CHECK(neg_negative_overflow<uint8_t>(i) == (y < 0));
            CHECK(neg_positive_overflow<uint8_t>(i) == (y > UINT8_MAX));
            CHECK(neg_overflow<uint8_t>(i) == ((y < 0) || (y > UINT8_MAX)));
        }
    }
}

TEST_CASE("Safer negation", "[integer]") 
{
    GIVEN("INT_MIN")
    {
        WHEN("Negation overflows back to INT_MIN")
        AND_WHEN("Sometimes not, because signed overflow is undefined")
            CHECK(-INT_MIN == INT_MIN);

        auto y = neg(INT_MIN);

        CHECK(y.value() == INT_MAX+1u);
        auto min = y.min;
        auto max = y.max;
        CHECK(min == -INT_MAX);
        CHECK(max == INT_MAX+1u);
    }

    GIVEN("UINT_MAX")
    {
        WHEN("Negation overflows to 1")
            CHECK(-UINT_MAX == 1);

        auto y = neg(UINT_MAX);

        CHECK(y.value() == -intmax_t(UINT_MAX));
        auto min = y.min;
        auto max = y.max;
        CHECK(min == -intmax_t(UINT_MAX));
        CHECK(max == 0);
    }
}
