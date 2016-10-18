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

struct IsMul4 : IsMul2, Failure
{
    bool operator()(int x)
    {
        return x%4 == 0;
    }
};

struct IsMul6 : IsMul2
{
    bool operator()(int x)
    {
        return x%6 == 0;
    }
};

using IntF = explicit_subtype<int, Failure>;
using Int2 = explicit_subtype<int, IsMul2>;
using Int4 = explicit_subtype<int, IsMul4>;
using Int6 = explicit_subtype<int, IsMul6>;

void use2(Int2)
{
}

void use4(Int4)
{
}

constexpr Int2 CONST_INT2(unsafe_forward(2));
constexpr Int4 CONST_INT4(unsafe_forward(4));

constexpr Int2 CONST_INT2_1(CONST_INT2);
constexpr Int2 CONST_INT2_2(CONST_INT4);

struct Test
{
    Int2 m_a, m_b;
    constexpr Test(Int2 a, Int4 b) : m_a(a), m_b(b) {}
};

constexpr Test CONST_TEST(CONST_INT2, CONST_INT4);

TEST_CASE("Explicit subtype construction", "[subtype]")
{
    REQUIRE(detail::is_subtype<IntF>::value);
    REQUIRE(detail::is_subtype<Int2>::value);
    REQUIRE(detail::is_subtype<Int4>::value);
    REQUIRE(detail::is_subtype<Int6>::value);

    REQUIRE(!detail::is_subtype<int>::value);
    REQUIRE((!detail::is_subtype<int, int>::value));

    // construction from value
    
    REQUIRE_NOTHROW(Int2(0));
    REQUIRE_THROWS(Int2(1));
    REQUIRE_NOTHROW(Int2(2));

    REQUIRE_NOTHROW(Int4(0));
    REQUIRE_THROWS(Int4(1));
    REQUIRE_THROWS(Int4(2));
    REQUIRE_THROWS(Int4(3));
    REQUIRE_NOTHROW(Int4(4));

    // conversion

    Int2 a(2);
    Int4 b(4);
    REQUIRE_NOTHROW(Int2{a});
    REQUIRE_NOTHROW(Int2{b});

    REQUIRE_THROWS(Int4{a});
    REQUIRE_NOTHROW(Int4{b});

    // IntF can't be directly assigned any int, but can be converted from Int4
    // - tests that check is skipped when predicates are convertible
    for(int i = 0; i < 5; ++i)
        REQUIRE_THROWS(IntF{i});
    REQUIRE_THROWS (IntF{a});
    REQUIRE_NOTHROW(IntF{b});

    REQUIRE_THROWS(Int6{a});
    REQUIRE_THROWS(Int6{b});

    // implicit conversions

    REQUIRE_NOTHROW(use2(a));
    REQUIRE_NOTHROW(use2(b));

    REQUIRE_THROWS(use4(Int4(a)));
    REQUIRE_NOTHROW(use4(b));
}

TEST_CASE("Explicit subtype assignment", "[subtype]")
{
    Int4 const zero(0);
    Int2 const two(2);
    Int4 const four(4);

    {
        Int2 a(0);
        REQUIRE(a == zero);
        REQUIRE_NOTHROW(a = zero);
        REQUIRE(a == zero);
        REQUIRE_NOTHROW(a = two);
        REQUIRE(a == two);
        REQUIRE_NOTHROW(a = four);
        REQUIRE(a == four);
    }

    {
        Int4 a(0);
        REQUIRE(a == zero);
        REQUIRE_NOTHROW(a = zero);
        REQUIRE(a == zero);
        REQUIRE_THROWS(a = Int4(two));
        REQUIRE(a == zero);
        REQUIRE_NOTHROW(a = four);
        REQUIRE(a == four);
    }

    {
        IntF a(zero);
        REQUIRE(a == zero);
        REQUIRE_THROWS(a = IntF(two));
        REQUIRE(a == zero);
        REQUIRE_NOTHROW(a = four);
        REQUIRE(a == four);
    }

    {
        Int6 a(6);
        REQUIRE_THROWS(a = Int6(two));
        REQUIRE_THROWS(a = Int6(four));
    }
}

// --

struct Td : subtype_traits<int, IsMul4>
{
    static constexpr conv_level unsafe_construct = conv_level::disable;
    static constexpr conv_level unsafe_convert = conv_level::disable;
};
struct Te : subtype_traits<int, IsMul4>
{
    static constexpr conv_level unsafe_construct = conv_level::enable;
    static constexpr conv_level unsafe_convert = conv_level::enable;
};
struct Ti : subtype_traits<int, IsMul4>
{
    static constexpr conv_level unsafe_construct = conv_level::implicit;
    static constexpr conv_level unsafe_convert = conv_level::implicit;
};

using I4d = subtype<int, IsMul4, Td>;
using I4e = subtype<int, IsMul4, Te>;
using I4i = subtype<int, IsMul4, Ti>;

void fd(I4d) {}
void fe(I4e) {}
void fi(I4i) {}

TEST_CASE("Traits", "[subtype]")
{
    //fd(4);
    //fd(I4d(4));

    //fe(4);
    fe(I4e(4));

    fi(4);
    fi(I4i(4));

    Int2 four(4);

    //fd(four);
    //fd(I4d(four));

    //fe(four);
    fe(I4e(four));

    fi(four);
    fi(I4i(four));
}
