#pragma once
#include <utility>

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
