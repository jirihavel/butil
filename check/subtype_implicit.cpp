#include <butil/subtype.h>

#include <cstdlib>

#include <catch.hpp>

using namespace butil;

template<typename T, typename P, typename Tp, typename Q, typename Tq>
bool operator==(subtype<T, P, Tp> const & a, subtype<T, Q, Tq> const & b)
{
    return a.value() == b.value();
}

struct Failure
{
    bool operator()(int)
    {
        return false;
    }
};

struct IsMul2
{
    bool operator()(int x)
    {
        return x%2 == 0;
    }
};

struct IsMul4
    : public IsMul2, public Failure
{
    bool operator()(int x)
    {
        return x%4 == 0;
    }
};

struct IsMul6
    : public IsMul2
{
    bool operator()(int x)
    {
        return x%6 == 0;
    }
};

using IntF = implicit_subtype<int, Failure>;
using Int2 = implicit_subtype<int, IsMul2>;
using Int4 = implicit_subtype<int, IsMul4>;
using Int6 = implicit_subtype<int, IsMul6>;

TEST_CASE("Subtype construction", "[subtype]")
{
    REQUIRE_NOTHROW(Int2{0});
    REQUIRE_THROWS(Int2{1});
    REQUIRE_NOTHROW(Int2{2});

    REQUIRE_NOTHROW(Int4{0});
    REQUIRE_THROWS(Int4{1});
    REQUIRE_THROWS(Int4{2});
    REQUIRE_THROWS(Int4{3});
    REQUIRE_NOTHROW(Int4{4});

    Int2 a(2);
    Int4 b(4);

    REQUIRE_NOTHROW(Int2{a});
    REQUIRE_NOTHROW(Int2{b});

    REQUIRE_THROWS(Int4{a});
    REQUIRE_NOTHROW(Int4{b});

    // IntF can't be directly assigned any int, but can be converted from Int4
    for(int i = 0; i < 5; ++i)
        REQUIRE_THROWS(IntF{i});
    REQUIRE_THROWS(IntF{a});
    REQUIRE_NOTHROW(IntF{b});

    REQUIRE_THROWS(Int6{b});
}

TEST_CASE("Subtype assignment", "[subtype]")
{
    {
        Int2 a(0);
        REQUIRE_NOTHROW(a = 0);
        REQUIRE(a == 0);
        REQUIRE_THROWS(a = 1);
        REQUIRE(a == 0);
        REQUIRE_NOTHROW(a = 2);
        REQUIRE(a == 2);
    }
    {
        Int4 a(0);
        REQUIRE_NOTHROW(a = 0);
        REQUIRE(a == 0);
        REQUIRE_THROWS(a = 1);
        REQUIRE(a == 0);
        REQUIRE_THROWS(a = 2);
        REQUIRE(a == 0);
        REQUIRE_THROWS(a = 3);
        REQUIRE(a == 0);
        REQUIRE_NOTHROW(a = 4);
        REQUIRE(a == 4);
    }
    {
        IntF a(Int4(0));
        REQUIRE_THROWS(a = Int2(2));
        REQUIRE(a == 0);
        REQUIRE_NOTHROW(a = Int4(4));
        REQUIRE(a == 4);
    }
    {
        Int6 a(6);
        REQUIRE_THROWS(a = Int2(2));
        REQUIRE(a == 6);
        REQUIRE_THROWS(a = Int4(4));
        REQUIRE(a == 6);
    }
}
