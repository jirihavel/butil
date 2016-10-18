#include <catch.hpp>

#include <butil/float/cmp.h>

using namespace butil::floating;

TEST_CASE("Float absolute comparison", "[float]")
{
    float const epsilon = 0.25f;
    CmpAbs<float> cmp(epsilon);

    CHECK(cmp.eq(0.0f, -0.0f));
    CHECK(cmp.eq(-0.0f, 0.0f));

    CHECK(cmp.eq(1.0f, 1.0f));
    CHECK(cmp.eq(1.0f, 1.0f+epsilon));
    CHECK(cmp.eq(1.0f+epsilon, 1.0f));
    CHECK(cmp.ne(1.0f, std::nextafter(1.0f+epsilon, FLT_MAX)));
    CHECK(cmp.ne(std::nextafter(1.0f+epsilon, FLT_MAX), 1.0f));

    CHECK(cmp.ne(0.0f, +INFINITY));
    CHECK(cmp.ne(0.0f, -INFINITY));
    CHECK(cmp.ne(+INFINITY, 0.0f));
    CHECK(cmp.ne(-INFINITY, 0.0f));

    CHECK(cmp.eq(+INFINITY, +INFINITY));
    CHECK(cmp.eq(-INFINITY, -INFINITY));
    CHECK(cmp.ne(+INFINITY, -INFINITY));
    CHECK(cmp.ne(-INFINITY, +INFINITY));

    CHECK(cmp.ne(NAN, 0.0f));
    CHECK(cmp.ne(NAN, +INFINITY));
    CHECK(cmp.ne(NAN, -INFINITY));
    CHECK(cmp.ne(0.0f, NAN));
    CHECK(cmp.ne(+INFINITY, NAN));
    CHECK(cmp.ne(-INFINITY, NAN));

    CmpAbs<float> cmp_inf(INFINITY);
    CHECK(cmp_inf.eq(0.0f, 10000.0f));
    CHECK(cmp_inf.eq(0.0f, +INFINITY));
    CHECK(cmp_inf.eq(0.0f, -INFINITY));
    CHECK(cmp_inf.eq(+INFINITY, +INFINITY));
    CHECK(cmp_inf.eq(-INFINITY, -INFINITY));
    CHECK(cmp_inf.eq(+INFINITY, -INFINITY));
    CHECK(cmp_inf.eq(-INFINITY, +INFINITY));

    CHECK(cmp_inf.ne(NAN, 0.0f));
    CHECK(cmp_inf.ne(NAN, +INFINITY));
    CHECK(cmp_inf.ne(NAN, -INFINITY));
    CHECK(cmp_inf.ne(0.0f, NAN));
    CHECK(cmp_inf.ne(+INFINITY, NAN));
    CHECK(cmp_inf.ne(-INFINITY, NAN));

    CmpAbs<float> cmp_zero(0.0f);

    CHECK(cmp_zero.eq(0.0f, -0.0f));
    CHECK(cmp_zero.eq(-0.0f, 0.0f));
}

TEST_CASE("Float relative comparison", "[float]")
{
    CmpRel<float> cmp(FLT_EPSILON);

    CHECK(cmp.eq(0.0f, -0.0f));
    CHECK(cmp.eq(-0.0f, 0.0f));

    CHECK(cmp.eq(1.0f, 1.0f));
    CHECK(cmp.eq(1.0f, 1.0f+FLT_EPSILON));
    CHECK(cmp.eq(1.0f+FLT_EPSILON, 1.0f));
    CHECK(cmp.ne(1.0f, std::nextafter(1.0f+FLT_EPSILON, 2.0f)));
    CHECK(cmp.ne(std::nextafter(1.0f+FLT_EPSILON, 2.0f), 1.0f));

    CHECK(cmp.ne(0.0f, +INFINITY));
    CHECK(cmp.ne(0.0f, -INFINITY));
    CHECK(cmp.ne(+INFINITY, 0.0f));
    CHECK(cmp.ne(-INFINITY, 0.0f));

    CHECK(cmp.eq(+INFINITY, +INFINITY));
    CHECK(cmp.eq(-INFINITY, -INFINITY));
    CHECK(cmp.ne(+INFINITY, -INFINITY));
    CHECK(cmp.ne(-INFINITY, +INFINITY));

    CHECK(cmp.ne(NAN, 0.0f));
    CHECK(cmp.ne(NAN, +INFINITY));
    CHECK(cmp.ne(NAN, -INFINITY));
    CHECK(cmp.ne(0.0f, NAN));
    CHECK(cmp.ne(+INFINITY, NAN));
    CHECK(cmp.ne(-INFINITY, NAN));

    CmpRel<float> cmp_inf(INFINITY);
    CHECK(cmp_inf.eq(0.0f, 10000.0f));
    CHECK(cmp_inf.eq(0.0f, +INFINITY));
    CHECK(cmp_inf.eq(0.0f, -INFINITY));
    CHECK(cmp_inf.eq(+INFINITY, +INFINITY));
    CHECK(cmp_inf.eq(-INFINITY, -INFINITY));
    CHECK(cmp_inf.eq(+INFINITY, -INFINITY));
    CHECK(cmp_inf.eq(-INFINITY, +INFINITY));

    CHECK(cmp_inf.ne(NAN, 0.0f));
    CHECK(cmp_inf.ne(NAN, +INFINITY));
    CHECK(cmp_inf.ne(NAN, -INFINITY));
    CHECK(cmp_inf.ne(0.0f, NAN));
    CHECK(cmp_inf.ne(+INFINITY, NAN));
    CHECK(cmp_inf.ne(-INFINITY, NAN));

    CmpRel<float> cmp_zero(0.0f);

    CHECK(cmp_zero.eq(0.0f, -0.0f));
    CHECK(cmp_zero.eq(-0.0f, 0.0f));
}
