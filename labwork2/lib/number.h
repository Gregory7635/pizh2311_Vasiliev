// labwork2/lib/number.h
#pragma once
#include <cinttypes>
#include <iostream>
#include <array>
#include <string>

struct uint2022_t {
    static const size_t NUM_BITS = 2022;
    static const size_t NUM_WORDS = (NUM_BITS + 31) / 32;
    std::array<uint32_t, NUM_WORDS> parts;

    uint2022_t() {
        parts.fill(0);
    }
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);
uint2022_t from_string(const char* buff);
uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);
uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);
uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);
uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);
bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);
bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);
std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);