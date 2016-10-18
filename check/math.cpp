#include <butil/inv.h>
#include <butil/ipow.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("Inverse", "[math]")
{
    CHECK(typeid(inv(1.0f)) == typeid(float));
    CHECK(typeid(inv(1.0)) == typeid(double));
    CHECK(typeid(inv(1)) == typeid(double));

    CHECK(inv(1) == Approx(1));
    CHECK(inv(2) == Approx(0.5));
    CHECK(inv(4) == Approx(0.25));
}

TEST_CASE("Integral power", "[math]")
{
    CHECK(upow(2, 0) == 1);
    CHECK(upow(2, 1) == 2);
    CHECK(upow(2, 2) == 4);
}

TEST_CASE("Rational power", "[math]")
{
    CHECK(ipow_<0>(2) == 1);
    CHECK(ipow_<1>(2) == 2);
    CHECK(ipow_<2>(2) == 4);

    CHECK((ipow_<2,2>(2) == 2));
    CHECK((typeid(ipow_<2,2>(2)) == typeid(int)));

    CHECK(ipow_<-1>(2) == Approx(0.5));
    CHECK(typeid(ipow_<-1>(2)) == typeid(double));

    CHECK((ipow_<1,2>(2) == Approx(sqrt(2))));
    CHECK((ipow_<1,3>(2) == Approx(cbrt(2))));
    CHECK((ipow_<1,4>(2) == Approx(sqrt(sqrt(2)))));
}

TEST_CASE("Square", "[math]")
{
    CHECK(typeid(sqr(1)) == typeid(int));
    CHECK(typeid(sqr(1.0)) == typeid(double));
    CHECK(typeid(sqr(1.0f)) == typeid(float));

    CHECK(sqr(1) == 1);
    CHECK(sqr(2) == 4);
    CHECK(sqr(3) == 9);
}
