#include <lib/number.h>
#include <iostream>

int main() {

    uint2022_t a = from_uint(15);
    uint2022_t b = from_uint(3);
    uint2022_t c = from_string("100");
    uint2022_t d = from_string("25");

    std::cout << "15 + 3 = " << (a + b) << "\n";
    std::cout << "15 - 3 = " << (a - b) << "\n";
    std::cout << "15 * 3 = " << (a * b) << "\n";


    std::cout << "100 + 25 = " << (c + d) << "\n";
    std::cout << "100 - 25 = " << (c - d) << "\n";
    std::cout << "100 * 25 = " << (c * d) << "\n";



    std::cout << "15 == 3? " << (a == b ? "true" : "false") << "\n";
    std::cout << "15 != 3? " << (a != b ? "true" : "false") << "\n";

    return 0;
}