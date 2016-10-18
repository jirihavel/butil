#define GSL_FAIL_FAST_THROWS
#include <butil/tagged_ptr.h>

#include <catch.hpp>

using namespace butil;

TEST_CASE("Tagged pointer", "[tagged_ptr]")
{
    int data = 0;

    using Ptr = tagged_ptr<int, 1>;

    CHECK_NOTHROW(Ptr{});
    CHECK_NOTHROW(Ptr(nullptr));
    CHECK_NOTHROW(Ptr{&data});
    CHECK_THROWS (Ptr(pun::u2p<int>(1)));

    Ptr ptr(&data);

    using Tag = Ptr::tag_type;
    for(Tag t = 0; t <= Ptr::tag_max; ++t)
    {
        CHECK_NOTHROW(ptr.set_tag(t));
        CHECK(ptr.get() == &data);
        CHECK(ptr.get_tag() == t);
    }
    CHECK_THROWS(ptr.set_tag(Ptr::tag_max+1));
}
