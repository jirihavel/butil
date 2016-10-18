#include <butil/string.h>

#include <catch.hpp>

using namespace std::literals;
using namespace butil;

TEST_CASE("String concatenation", "[string]")
{
    CHECK(str_cat('a', 'b', 'c', 'd') == "abcd");
    CHECK(str_cat("1"s, "2", '3', 4) == "1234");
}
