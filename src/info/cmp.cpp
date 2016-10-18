#include <butil/integer/cmp.h>

#include <cstdint>
#include <cstdlib>

#include <iostream>

template<typename A, typename B>
void print()
{
#pragma GCC diagnostic ignored "-Wsign-compare"
    A a = -1;
    B b = -1;

    std::cout << (a<b) << (+a<+b) << butil::integer::cmp::common_type_safe<A, B>::value << ' ';
#pragma GCC diagnostic pop
}

int main()
{
    std::cout << "i | u  8  16  32  64\n";

    std::cout << " 8 : ";
    print<int8_t, uint8_t>();
    print<int8_t, uint16_t>();
    print<int8_t, uint32_t>();
    print<int8_t, uint64_t>();
    std::cout << std::endl;

    std::cout << "16 : ";
    print<int16_t, uint8_t>();
    print<int16_t, uint16_t>();
    print<int16_t, uint32_t>();
    print<int16_t, uint64_t>();
    std::cout << std::endl;

    std::cout << "32 : ";
    print<int32_t, uint8_t>();
    print<int32_t, uint16_t>();
    print<int32_t, uint32_t>();
    print<int32_t, uint64_t>();
    std::cout << std::endl;

    std::cout << "64 : ";
    print<int64_t, uint8_t>();
    print<int64_t, uint16_t>();
    print<int64_t, uint32_t>();
    print<int64_t, uint64_t>();
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
