
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
bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int i = lhs.NUM_WORDS - 1; i >= 0; i--) {
        if (lhs.parts[i] < rhs.parts[i]) return true;
        if (lhs.parts[i] > rhs.parts[i]) return false;
    }
    return false;
}
uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;
    uint32_t carry = 0;

    for (size_t i = 0; i < result.NUM_WORDS; i++) {
        uint64_t sum = (uint64_t)lhs.parts[i] + rhs.parts[i] + carry;
        result.parts[i] = sum & 0xFFFFFFFF;
        carry = sum >> 32;
    }

    if (carry != 0) {
        throw std::overflow_error("Overflow in uint2022_t addition");
    }

    return result;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs < rhs) {
        throw std::underflow_error("Underflow in uint2022_t subtraction");
    }

    uint2022_t result;
    uint32_t borrow = 0;

    for (size_t i = 0; i < result.NUM_WORDS; i++) {
        uint64_t diff = (uint64_t)lhs.parts[i] - rhs.parts[i] - borrow;
        result.parts[i] = diff & 0xFFFFFFFF;
        borrow = (diff >> 63) & 1;
    }

    return result;
}


uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;

    for (size_t i = 0; i < lhs.NUM_WORDS; i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < rhs.NUM_WORDS && i + j < result.NUM_WORDS; j++) {
            uint64_t product = (uint64_t)lhs.parts[i] * rhs.parts[j] + result.parts[i + j] + carry;
            result.parts[i + j] = product & 0xFFFFFFFF;
            carry = product >> 32;
        }

                                             
        if (i + rhs.NUM_WORDS < result.NUM_WORDS && carry != 0) {
            uint64_t sum = (uint64_t)result.parts[i + rhs.NUM_WORDS] + carry;
            result.parts[i + rhs.NUM_WORDS] = sum & 0xFFFFFFFF;
            if ((sum >> 32) != 0) {
                throw std::overflow_error("Overflow in uint2022_t multiplication");
            }
        }
        else if (i + rhs.NUM_WORDS >= result.NUM_WORDS && carry != 0) {
            throw std::overflow_error("Overflow in uint2022_t multiplication");
        }
    }

    return result;
}
bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (size_t i = 0; i < lhs.NUM_WORDS; i++) {
        if (lhs.parts[i] != rhs.parts[i]) {
            return false;
        }
    }
    return true;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (rhs == uint2022_t()) {

        return uint2022_t();
    }

    uint2022_t quotient;
    uint2022_t remainder = lhs;

    while (!(remainder < rhs)) {
        uint2022_t temp_divisor = rhs;
        uint2022_t multiple = from_uint(1);

        //                   ,         "       "          
        while (!(remainder < (temp_divisor + temp_divisor))) {
            temp_divisor = temp_divisor + temp_divisor;
            multiple = multiple + multiple;
        }

        remainder = remainder - temp_divisor;
        quotient = quotient + multiple;
    }

    return quotient;
}









