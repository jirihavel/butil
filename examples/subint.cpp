#include <butil/subint.h>
#include <butil/pun.h>
#include <butil/integer/io.h>

#include <iostream>

// Pointers

void not_null_ptr(butil::not_null<int*> ptr)
{
    std::cout << ptr << " -> " << *ptr << "\n";
}

struct A { int a; int b; };

// not_null strict does not allow conversions
void not_null_aptr(butil::not_null_strict<A*> ptr)
{
    std::cout << ptr.value() << " -> (" << (*ptr).a << ", " << ptr->b << ")\n";
}

template<uintmax_t Align>
struct is_aligned
{
    static_assert((Align > 0) && (((Align-1)&Align) == 0), "");
    template<typename T>
    bool operator()(T const * x) const noexcept
    {
        return (butil::pun::p2u(x)&(Align-1)) == 0;
    }
};

void aligned_ptr(butil::implicit_subtype<int*, is_aligned<4>> ptr)
{
    std::cout << ptr << " is aligned to 4B\n";
}

// Integers

template<typename Interval>
using positive_subinterval = butil::integer::interval_meet<Interval, butil::integer::interval<uintmax_t, 1>>;

void positive_int(butil::implicit_subint<positive_subinterval<int>> val)
{
    std::cout << butil::io::dbg(val) << " is in [1, INT_MAX]\n";
}

void unsigned_int(butil::implicit_subint<unsigned> val)
{
    std::cout << butil::io::dbg(val) << " is unsigned\n";
}

template<typename A, typename B>
void int_add(A a, B b)
{
    std::cout << butil::io::dbg(a) << " + " << butil::io::dbg(b) << " = " << butil::io::dbg(butil::integer::add(a, b)) << "\n";
}

int main()
{
    // pointers
    //not_null_ptr(nullptr); // compile time error
    //not_null_ptr((int*)NULL); // runtime error
    int x;
    not_null_ptr(&x);
    A x1 = { 0, 1 };
    not_null_aptr(butil::not_null<A*>(&x1));
    aligned_ptr(&x);

    // Integers
    // positive_int(0); // runtime error
    positive_int(1);
    //unsigned_int(-1); // runtime error, prevents wrap
    unsigned_int(0);

    int_add(butil::implicit_subint<uint8_t>(1), butil::implicit_subint<uint8_t>(1));

    return EXIT_SUCCESS;
};
