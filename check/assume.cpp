#define GSL_FAIL_FAST_THROWS
#include <butil/assume.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("Assumptions", "[assume]")
{
    int * variable = nullptr;

    CHECK_NOTHROW(assume_null = nullptr);
    CHECK_NOTHROW(assume_null = variable);
    CHECK_THROWS (assume_null = &variable);

    CHECK_NOTHROW(assume_true = true);
    CHECK_NOTHROW(assume_false = false);

    CHECK_THROWS(assume_true = false);
    CHECK_THROWS(assume_false = true);

    CHECK_NOTHROW(std::tie(assume_true, assume_false) = std::make_pair(true, false));
    CHECK_THROWS (std::tie(assume_true, assume_false) = std::make_pair(false, false));
    CHECK_THROWS (std::tie(assume_true, assume_false) = std::make_pair(false, true));
    CHECK_THROWS (std::tie(assume_true, assume_false) = std::make_pair(true, true));

    CHECK_NOTHROW(assume(0) = 0);
    CHECK_THROWS (assume(0) = 1);
}
