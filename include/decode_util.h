#pragma once
#include <utility>
#include <string>
#include <cassert>

constexpr uint32_t Bits(uint32_t value, int hi, int lo = -1)
{
    if (lo == -1) lo = hi;
    return (value >> lo) & ((1u << (hi - lo + 1)) - 1);
}

static uint32_t EndianSwapper(uint32_t value)
{ 
    uint32_t leftmost_byte;
    uint32_t left_middle_byle;
    uint32_t right_middle_byte;
    uint32_t rightmost_byte;

    uint32_t result;

    leftmost_byte = (value & 0x000000FF) >> 0;
    left_middle_byle = (value & 0x0000FF00) >> 8;
    right_middle_byte = (value & 0x00FF0000) >> 16;
    rightmost_byte = (value & 0xFF000000) >> 24;

    leftmost_byte <<= 24;
    left_middle_byle <<= 16;

    right_middle_byte <<= 8;
    rightmost_byte <<= 0;

    result = (leftmost_byte | left_middle_byle |
        right_middle_byte | rightmost_byte);

    return result;
}
static std::string GetRegName(uint8_t registry, bool width)
{
    if (registry == 31)
        return "sp";
    if(width)
        return std::string("x") + std::to_string(registry);

    return std::string("w") + std::to_string(registry);
}

// Sign-extend an M-bit number x to N bits
static int64_t SignExtend(uint64_t x, int M, int N) {
    assert(N >= M);

    // Extract the sign bit
    bool sign = (x >> (M - 1)) & 1;

    if (sign) {
        // If sign bit is 1, extend with 1s
        uint64_t extend_mask = ((1ULL << (N - M)) - 1) << M;
        x |= extend_mask;
    }
    // Else sign bit is 0, nothing to do (upper bits already 0)
    return static_cast<int64_t>(x);
}

static uint64_t Zeros(int N) {
    assert(N <= 64); // can't fit more than 64 bits in uint64_t
    return 0;        // just return all zeros
}