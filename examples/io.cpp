#include <butil/io.h>
#include <butil/io/yaml.h>

#include <cstdlib>

#include <iostream>
#include <vector>

namespace io = butil::io;

struct Foo {};

std::ostream & operator<<(std::ostream & stream, Foo const &)
{
    return stream 
        << "a: " << 1
        << io::endl << "b: " << 2
        << io::endl << "c: " << 3;
}

struct Bar
{
    Foo foo;
    std::vector<Foo> vec;
};

std::ostream & operator<<(std::ostream & stream, Bar const & bar)
{
    using namespace io::yaml;
    io::indenter indent(stream);
    return stream
        << "foo:" << indent.endl(1) << bar.foo << indent.endl()
        << "vec:" << indent.endl(1) << bar.vec;
}

int main()
{
    // debug printing
    std::cout
        << "val: " << 1 << ", debug: " << io::dbg(1) << std::endl;

    Foo foo;
    // indentation
    // - indenter stores old indentation state of the stream. It will be restored on scope end.
    io::indenter indent(std::cout);
    // - each indent(i) prints indentation and sets state
    std::cout << indent << foo << '\n'
        << indent(1) << foo << '\n'
        << indent(2) << foo << '\n';
    // - indentation stays set, the second line of Foo will be indented;
    // - printing indent will reset it
    //std::cout << Foo() << std::endl;
    // containers
    std::vector<int> data = { 0, 1, 2 };
    // - simple printing
    std::cout << "vec = {" << io::intersperse(std::cref(data), ", ") << "}\n";
    // - itemized
    {
        auto const endl = indent.endl(1); // endl(i) prints \n and indents by i levels
        std::cout << "vec:" << endl
            << io::itemize(std::cref(data), indent.item("- "), endl) << '\n';
    }
    // - use operator for vector
    // -- streaming indent.endl(1) prints indentation and sets same indentation for following prints
    //std::cout << "vec:" << indent.endl(1) << data << '\n';
    // Nested printing :
    Bar bar;
    bar.vec.resize(2);
    std::cout << "bar:" << indent.endl(1) << bar << std::endl;
    return EXIT_SUCCESS;
}
