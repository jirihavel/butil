#include <butil/io/indentation.h>

#include <catch.hpp>
#include <iostream>

namespace io = butil::io;

struct A {};
struct B {};
struct Z {};
struct L {};

std::ostream & operator<<(std::ostream & stream, A const &)
{
    io::indenter indent(stream);
    return stream << "p: '" << indent.prefix() << "'\n"
        << indent << "s: '" << indent.step() << "'\n"
        << indent << "d: " << indent.depth();
}

std::string const refa = "p: ''\ns: '  '\nd: 0";

std::ostream & operator<<(std::ostream & stream, B const &)
{
    io::indenter indent(stream);
    return stream << "p: '" << indent.prefix() << "'\n"
        << indent << "s: '" << indent.step() << "'\n"
        << indent << "d: " << indent.depth() << '\n'
        << indent << "a:" << indent.endl(1) << A();
}

std::string const refb = "p: ''\ns: '  '\nd: 0\na:\n  p: ''\n  s: '  '\n  d: 1";

std::ostream & operator<<(std::ostream & stream, L const &)
{
    io::indenter indent(stream);
    auto const endl = indent.endl();
    auto const item = indent.item("- ");
    return stream
        << item << B() << endl
        << item << B() << endl
        << item << B() << endl
        << item << B();
}

std::ostream & operator<<(std::ostream & stream, Z const &)
{
    io::indenter indent(stream);
    auto const endl = indent.endl();
    auto const nest = indent.endl(1);
    return stream
        << "a:" << nest << A() << endl
        << "b:" << nest << B() << endl
        << "l:" << nest << L() << endl
        << "p: '" << indent.prefix() << '\'' << endl
        << "s: '" << indent.step()   << '\'' << endl
        << "d: "  << indent.depth();
}

TEST_CASE("indentation", "[indentation]")
{
    CHECK(refa == boost::lexical_cast<std::string>(A()));
    CHECK(refb == boost::lexical_cast<std::string>(B()));
    std::cout << Z() << std::endl;
}
