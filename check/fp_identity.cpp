#include <butil/fp/identity.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("FP identity object", "[fp]")
{
    using namespace std::literals;

    CHECK(fp::identity(0) == 0);
    CHECK(fp::identity(1) == 1);
    CHECK(fp::identity("abc"s) == "abc");
}
