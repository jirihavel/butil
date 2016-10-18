#include <butil/subtype.h>
#include <butil/fp/constant.h>

#include <butil/io/debug.h>

#include <iostream>

using namespace butil;

struct TagA : fp::constant_true_t {};
struct TagB : fp::constant_true_t {};

using IntA = explicit_subtype<int, TagA>;
using IntB = explicit_subtype<int, TagB>;

std::ostream & operator<<(std::ostream & o, IntA const & v)
{
    return o << v.value();
}

void f(IntA)
{
    std::cout << "Version A called\n";
}

void f(IntB)
{
    std::cout << "Version B called\n";
}

void g(int)
{
}

int main()
{
    static_assert(std::is_base_of<subtype_base, IntA>::value, "");
    static_assert(detail::is_subtype<IntA>::value == true, "");
    static_assert(detail::is_subtype<IntA &>::value == true, "");
    static_assert(detail::is_subtype<IntA const &>::value == true, "");
//    static_assert(detail::is_subtype<int>::value == false, "");
//    static_assert(detail::is_subtype<int &>::value == false, "");
//    static_assert(detail::is_subtype<int const &>::value == false, "");

    static_assert(IntA::traits_type::unsafe_construct == conv_level::enable, "");

//    static_assert(detail::subtype_conversion<IntA, int>::safe::value == false, "");

    IntA a;
    IntA b(1);

    IntA c = b;

    // IntB d = b; // fails to compile, requires implicit conversion
    IntB e(b); // explicit conversion

    f(b);// version A
    f(e);// version B

    // g(b); // fails to compile, requires implicit conversion
    g(int(b));

    std::cout << butil::io::dbg(a) << std::endl;

    return EXIT_SUCCESS;
};
