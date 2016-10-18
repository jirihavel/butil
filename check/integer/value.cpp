#include <catch.hpp>

#include <butil/integer/value.h>

using namespace butil::integer;

TEST_CASE("Value to integer conversion", "[integer][value]")
{
    GIVEN("Safe but possibly big value")
    {
        intset_value<interval<intmax_t, 0>> value(0);

        // commented assignments do not compile

        //char c = value;
        //signed char sc = value;
        //unsigned char uc = value;

        //signed short ss = value;
        //unsigned short us = value;

        //signed int si = value;
        //unsigned int ui = value;

        //signed long sl = value;
        //unsigned long ul = value;

        signed long long sll = value;
        unsigned long long ull = value;

        char foo = value.value();
    }

    GIVEN("Possibly unsafe value")
    {
        intset_value<interval<int, 0>, value_flags::overflow> value(0);

        //int x = value; - does not compile
        int y = value.value();
    }
}

TEST_CASE("Value cast", "[integer][value]")
{
    GIVEN("Guaranteed input")
    {
        // [0, 255]
        intset_value<interval<unsigned char>> ten(10);

        WHEN("Safe conversion")
        {
            auto y = value_cast<interval<int>>(ten);
            static_assert(y.flag_mask == value_flags::none, "");
            CHECK(y.flags() == value_flags::none);
            CHECK(y.value() == 10);
        }

        WHEN("Potential oveflow")
        {
            auto y = value_cast<interval<signed char>>(ten);
            static_assert(y.flag_mask == value_flags::overflow, "");
            CHECK(y.flags() == value_flags::none);
            CHECK(y.value() == 10);
        }

        WHEN("Overflow")
        {
            auto y = value_cast<interval<signed char, 0, 5>>(ten);
            static_assert(y.flag_mask == value_flags::overflow, "");
            CHECK(y.flags() == value_flags::overflow);
            //CHECK(y.value() == 10); !!! assert failure
        }
    }

    GIVEN("Unsafe input")
    {
        intset_value<interval<unsigned char>, value_flags::domain_error> ten(10);

        WHEN("Safe conversion")
        {
            auto y = value_cast<interval<int>>(ten);
            static_assert(y.flag_mask == value_flags::domain_error, "");
            CHECK(y.flags() == value_flags::none);
            CHECK(y.value() == 10);
        }

        WHEN("Potential overflow")
        {
            auto y = value_cast<interval<signed char>>(ten);
            static_assert(y.flag_mask == (value_flags::overflow|value_flags::domain_error), "");
            CHECK(y.flags() == value_flags::none);
            CHECK(y.value() == 10);
        }

        WHEN("Overflow")
        {
            auto y = value_cast<interval<signed char, 0, 5>>(ten);
            static_assert(y.flag_mask == (value_flags::overflow|value_flags::domain_error), "");
            CHECK(y.flags() == value_flags::overflow);
        }
    }

    GIVEN("Invalid input")
    {
        intset_value<interval<unsigned char>, value_flags::domain_error> ten(10, value_flags::domain_error);

        CHECK(ten.flags() == value_flags::domain_error);

        WHEN("Safe conversion")
        {
            auto y = value_cast<interval<int>>(ten);
            static_assert(y.flag_mask == value_flags::domain_error, "");
            CHECK(y.flags() == value_flags::domain_error);
        }

        WHEN("Potential overflow")
        {
            auto y = value_cast<interval<signed char>>(ten);
            static_assert(y.flag_mask == (value_flags::overflow|value_flags::domain_error), "");
            CHECK(y.flags() == value_flags::domain_error);
        }

        WHEN("Range error")
        {
            auto y = value_cast<interval<signed char, 0, 5>>(ten);
            static_assert(y.flag_mask == (value_flags::overflow|value_flags::domain_error), "");
            CHECK(y.flags() == value_flags::domain_error);
        }
    }
}
