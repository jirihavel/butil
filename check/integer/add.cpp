#include <catch.hpp>

#include <butil/integer/add.h>

using namespace butil::integer;

TEST_CASE("Addition overflow test functions", "[integer]")
{
    SECTION("signed")
    {
        for(int16_t i = INT8_MIN; i <= INT8_MAX; ++i)
        {
            for(int16_t j = INT8_MIN; j <= INT8_MAX; ++j)
            {
                CAPTURE(i);
                CAPTURE(j);
                int16_t const y = i+j;
                CHECK(add_negative_overflow<int8_t>(i, j) == (y < INT8_MIN));
                CHECK(add_positive_overflow<int8_t>(i, j) == (y > INT8_MAX));
                CHECK(add_overflow<int8_t>(i, j) == ((y < INT8_MIN) || (y > INT8_MAX)));
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
                CHECK(!add_negative_overflow<uint8_t>(i, j));
                bool const overflow = ((i+j) > UINT8_MAX);
                CHECK(add_positive_overflow<uint8_t>(i, j) == overflow);
                CHECK(add_overflow<uint8_t>(i, j) == overflow);
            }
        }
    }
}

TEST_CASE("Safer addition", "[integer]")
{
    GIVEN("Lots of different types")
    {
        std::integral_constant<int, 1> one;

        auto a = true; // [0, 1]
        auto b = one;  // [1, 1]
        auto c = 1;    // [INT_MIN, INT_MAX]
        auto d = '\1'; // [CHAR_MIN, CHAR_MAX]
        auto e = 1u;   // [O, UINT_MAX]

        auto y = add(true, one, 1, '\1', 1u);

        THEN("Their integral values are summed")
            CHECK(y.value() == 5);

        THEN("Boundaries are tracked")
        {
            auto min = y.min;
            auto max = y.max;
            CHECK(min == (intmax_t(0)+1+INT_MIN+CHAR_MIN));
            CHECK(max == (uintmax_t(1)+1+INT_MAX+CHAR_MAX+UINT_MAX));
        }
    }

    GIVEN("Mixed sign types")
    {
        auto a = -2;
        auto b = 1u;

        intmax_t correct = (intmax_t(a)+intmax_t(b));

        WHEN("Automatic conversion changes values")
        {
            auto y = a+b;
            CHECK(y != correct);
        }

        auto y = add(a, b);

        THEN("Values are summed correctly")
            CHECK(y.value() == correct);

        THEN("Boundaries are tracked")
        {
            auto min = y.min;
            auto max = y.max;
            CHECK(min == INT_MIN);
            CHECK(max == (intmax_t(INT_MAX)+UINT_MAX));
        }
    }

    GIVEN("Big values")
    {
        auto a = INT_MAX;
        auto b = UINT_MAX;

        intmax_t correct = (intmax_t(a)+intmax_t(b));

        WHEN("Normal addition overflows")
        {
            auto y = a+b;
            CHECK(y != correct);
        }

        auto y = add(a, b);

        THEN("Values are summed correctly")
            CHECK(y.value() == correct);

        THEN("Boundaries are tracked")
        {
            auto min = y.min;
            auto max = y.max;
            CHECK(min == INT_MIN);
            CHECK(max == (intmax_t(INT_MAX)+UINT_MAX));
        }
    }
}
