#include <catch.hpp>

#include <butil/integer/mul.h>

using namespace butil::integer;

TEST_CASE("mul uint8", "[integer]")
{
    for(uint16_t i = 0; i <= UINT8_MAX; ++i)
    {
        for(uint16_t j = 0; j <= UINT8_MAX; ++j)
        {
            uint16_t const y = i*j;
            CHECK(!mul_negative_overflow<uint8_t>(i, j));
            CHECK(mul_positive_overflow<uint8_t>(i, j) == (y > UINT8_MAX));
            CHECK(mul_overflow<uint8_t>(i, j) == (y > UINT8_MAX));
        }
    }
}

TEST_CASE("mul int8", "[integer]")
{
    for(int16_t i = INT8_MIN; i <= INT8_MAX; ++i)
    {
        for(int16_t j = INT8_MIN; j <= INT8_MAX; ++j)
        {
            int16_t const y = i*j;
            CHECK(mul_negative_overflow<int8_t>(i, j) == (y < INT8_MIN));
            CHECK(mul_positive_overflow<int8_t>(i, j) == (y > INT8_MAX));
            CHECK(mul_overflow<int8_t>(i, j) == ((y < INT8_MIN) || (y > INT8_MAX)));
        }
    }
}
