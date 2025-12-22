#pragma once
#include <utility>
#include <string>
#include <cassert>
#include <sstream>

constexpr uint32_t Bits(uint32_t value, int hi, int lo = -1)
{
    if (lo == -1) lo = hi;
    return (value >> lo) & ((1u << (hi - lo + 1)) - 1);
}

static uint32_t EndianSwapper(uint32_t v)
{
    return (v << 24) | ((v << 8) & 0x00FF0000) | ((v >> 8) & 0x0000FF00) | (v >> 24);
}
static std::string GetRegName(uint8_t registry, bool width, bool sp = true)
{
    if (registry == 31 && sp)
        return "sp";
    if(width)
        return std::string("x") + std::to_string(registry);

    return std::string("w") + std::to_string(registry);
}

// Sign-extend an M-bit number x to N bits
template <typename T>
static T SignExtend(T x, int M, int N) 
{
    assert(N >= M);

    // Extract the sign bit
    bool sign = (x >> (M - 1)) & 1;

    if (sign) {
        // If sign bit is 1, extend with 1s
        T extend_mask = ((1ULL << (N - M)) - 1) << M;
        x |= extend_mask;
    }
    return static_cast<T>(x);
}

static uint64_t Zeros(int N) 
{
    assert(N <= 64);
    return 0;
}

template <typename T>
static std::string ToHexFormat(T value)
{
    std::stringstream ss;

    if constexpr (std::is_signed_v<T>)
    {
        if (value < 0)
        {
            ss << "-0x" << std::hex << std::uppercase;
            ss << static_cast<std::make_unsigned_t<T>>(-value);
            return ss.str();
        }
        else
            ss << "0x" << std::hex << std::uppercase;
    }
    else
        ss << "0x" << std::hex << std::uppercase;

    ss << +static_cast<std::make_unsigned_t<T>>(value);
    return ss.str();
}


struct InstructionData
{
    uint32_t instruction;
    uint64_t va;
    uint32_t startVa;
    uint32_t vaLength;
};
