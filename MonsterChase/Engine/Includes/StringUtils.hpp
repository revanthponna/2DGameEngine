#pragma once

#include "String.hpp"

class StringUtils {

    public:

static      UInt8        CountDigit       (UInt64 pNum) noexcept;
static      String *     UInt2Str         (UInt64 pNum) noexcept;

static      UInt8        UInt2Str         (UInt64 pNum, char * pStr, UInt64 pLen) noexcept;

static      UInt64       CountNumChar     (const char * pStr) noexcept;

    private:

static constexpr UInt8 vNumAsciiArray[] {48, 49, 50, 51, 52, 53, 54, 55, 56, 57}; // ascii values for numbers 0-9. used in Int2Str.

    // this is a static class. Hence, dis-allowing stack instances or heap instances and copy operators

    DISALLOW_ON_STACK (StringUtils);
    DISALLOW_COPY (StringUtils);
};
