#include <butil/fp/ctype.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("FP ctype objects", "[fp]")
{
    for(int c = CHAR_MIN; c <= CHAR_MAX; ++c)
    {
        CHECK(!!std::isalnum(c) == fp::isalnum(c));
        CHECK(!!std::isalpha(c) == fp::isalpha(c));
        CHECK(!!std::isblank(c) == fp::isblank(c));
        CHECK(!!std::iscntrl(c) == fp::iscntrl(c));
        CHECK(!!std::isdigit(c) == fp::isdigit(c));
        CHECK(!!std::isgraph(c) == fp::isgraph(c));
        CHECK(!!std::islower(c) == fp::islower(c));
        CHECK(!!std::isprint(c) == fp::isprint(c));
        CHECK(!!std::ispunct(c) == fp::ispunct(c));
        CHECK(!!std::isspace(c) == fp::isspace(c));
        CHECK(!!std::isupper(c) == fp::isupper(c));
        CHECK(!!std::isxdigit(c) == fp::isxdigit(c));

        CHECK(std::tolower(c) == fp::tolower(c));
        CHECK(std::toupper(c) == fp::toupper(c));
    }
}
