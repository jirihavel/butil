#include <catch.hpp>
 
#include <butil/integer/div.h>

using namespace butil::integer;

TEST_CASE("divmod", "[integer]") 
{
    CHECK(int_divmod( 5,  2, mod_sign::positive) == std::make_pair( 2, 1));
    CHECK(int_divmod( 5, -2, mod_sign::positive) == std::make_pair(-2, 1));
    CHECK(int_divmod(-5,  2, mod_sign::positive) == std::make_pair(-3, 1));
    CHECK(int_divmod(-5, -2, mod_sign::positive) == std::make_pair( 3, 1));

    CHECK(int_divmod( 5,  2, mod_sign::dividend) == std::make_pair( 2,  1));
    CHECK(int_divmod( 5, -2, mod_sign::dividend) == std::make_pair(-2,  1));
    CHECK(int_divmod(-5,  2, mod_sign::dividend) == std::make_pair(-2, -1));
    CHECK(int_divmod(-5, -2, mod_sign::dividend) == std::make_pair( 2, -1));

    CHECK(int_divmod( 5,  2, mod_sign::divisor) == std::make_pair( 2,  1));
    CHECK(int_divmod( 5, -2, mod_sign::divisor) == std::make_pair(-3, -1));
    CHECK(int_divmod(-5,  2, mod_sign::divisor) == std::make_pair(-3,  1));
    CHECK(int_divmod(-5, -2, mod_sign::divisor) == std::make_pair( 2, -1));
}

mod_sign modes[3] = { mod_sign::positive, mod_sign::dividend, mod_sign::divisor };

TEST_CASE("div uint8", "[integer]") 
{
    for(int m = 0; m < 3; ++m)
    {
        for(int16_t i = 0; i <= UINT8_MAX; ++i)
        {
            for(int16_t j = 1; j <= UINT8_MAX; ++j)// j != 0
            {
                CHECK(!div_positive_overflow<uint8_t>(i, j));
                CHECK(!div_overflow<uint8_t>(i, j));
                uint8_t q, r;
                std::tie(q, r) = int_divmod<uint8_t>(i, j, modes[m]);
                CHECK(r < uint8_t(j));
                CHECK(i == j*q+r );
            }
        }
    }
}

TEST_CASE("div int8", "[integer]") 
{
    for(int16_t i = INT8_MIN; i <= INT8_MAX; ++i)
    {
        for(int16_t j = INT8_MIN; j <= INT8_MAX; ++j)
        {
            if(j != 0)
            {
                CHECK(div_positive_overflow<int8_t>(i, j) == ((i == INT8_MIN) && (j == -1)));
                CHECK(div_overflow<int8_t>(i, j) == div_positive_overflow<int8_t>(i, j));
                if(!div_overflow<int8_t>(i, j))
                {
                    int8_t q, r;
                    std::tie(q, r) = int_divmod<int8_t>(i, j, mod_sign::positive);
                    CHECK(r >= 0);
                    CHECK(r < abs(j));
                    CHECK(i == j*q+r);
                    std::tie(q, r) = int_divmod<int8_t>(i, j, mod_sign::dividend);
                    CHECK((i >= 0 ? r >= 0 : r <= 0));
                    CHECK(abs(r) >= 0);
                    CHECK(abs(r) < abs(j));
                    CHECK(i == j*q+r);
                    std::tie(q, r) = int_divmod<int8_t>(i, j, mod_sign::divisor);
                    CHECK((j >= 0 ? r >= 0 : r <= 0));
                    CHECK(abs(r) >= 0);
                    CHECK(abs(r) < abs(j));
                    CHECK(i == j*q+r);
                }
            }
        }
    }
}
