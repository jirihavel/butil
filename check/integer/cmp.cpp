#include <catch.hpp>

#include <butil/integer/cmp.h>

using namespace butil::integer::cmp;

// Supress annoying warnings
#pragma GCC diagnostic ignored "-Wsign-compare"

TEST_CASE("compare for equality", "[integer]")
{
    CHECK(-1 == UINT_MAX);
    CHECK(!eq(-1, UINT_MAX));
    CHECK( ne(-1, UINT_MAX));

    // same signedness
    CHECK(!eq(-1, 2));
    CHECK( ne(-1, 2));

    // safe conversion
    CHECK(!eq(-1ll, UINT_MAX));
    CHECK( ne(-1ll, UINT_MAX));

    CHECK(eq(false, '\0', 0, 0u, 0l, 0ul, 0ll, 0ull));
    CHECK(eq( true, '\1', 1, 1u, 1l, 1ul, 1ll, 1ull));
}

TEST_CASE("compare for nonequality", "[integer]")
{
    CHECK(-1 > 0u);
    CHECK(lt(-1, 0u));
    CHECK(gt(0u, -1));

    CHECK(le(-1, 0u));
    CHECK(le( 0, 0u));
    CHECK(ge(0u, -1));
    CHECK(ge(0u,  0));

    CHECK( lt(-1, 0u, 1));
    CHECK(!lt(-1, 0, 0u, 1));
    CHECK( le(-1, 0, 0u, 1));

    CHECK( gt(1, 0u, -1));
    CHECK(!gt(1, 0, 0u, -1));
    CHECK( ge(1, 0, 0u, -1));
}

TEST_CASE("intset traits", "[integer]")
{
    std::integral_constant<int, 1> one;

    CHECK(!eq(one, -1));
    CHECK(eq(one, 1));

    CHECK(ne(one, 0));
    CHECK(eq(true, '\1', one, 1, 1u, 1ul, 1ull));
}
