
#include "number.h"
#include <string>
#include <algorithm>

uint2022_t from_uint(uint32_t i) {
    uint2022_t result;
    result.parts[0] = i;
    return result;
}

uint2022_t from_string(const char* buff) {
    uint2022_t result;
    std::string str(buff);

    for (char c : str) {
        if (c < '0' || c > '9') {
            return result; 
        }
    }

    for (size_t i = 0; i < str.size(); i++) {
        uint32_t carry = 0;
        for (size_t j = 0; j < result.NUM_WORDS; j++) {
            uint64_t value = (uint64_t)result.parts[j] * 10 + carry;
            result.parts[j] = value & 0xFFFFFFFF;
            carry = value >> 32;
        }

        
        carry = str[i] - '0';
        for (size_t j = 0; j < result.NUM_WORDS && carry > 0; j++) {
            uint64_t value = (uint64_t)result.parts[j] + carry;
            result.parts[j] = value & 0xFFFFFFFF;
            carry = value >> 32;
        }
    }

    return result;
}


