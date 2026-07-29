
#pragma once
#include <cstdint>

namespace Theme
{
    // Store as uint32_t in Direct2D order: 0xRRGGBB (or 0xAARRGGBB)
    inline constexpr uint32_t Background   = 0x141416;  // RGB(20,20,22)
    inline constexpr uint32_t Surface      = 0x1C1C20;  // RGB(28,28,32)
    inline constexpr uint32_t Surface2     = 0x26262C;  // RGB(38,38,44)

    inline constexpr uint32_t Primary      = 0x007AFF;  // RGB(0,122,255)
    inline constexpr uint32_t Secondary    = 0x6C757D;  // RGB(108,117,125)

    inline constexpr uint32_t ButtonFillPrimary   = 0x007AFF;
    inline constexpr uint32_t ButtonFillSecondary = 0x2C2C34;

    inline constexpr uint32_t ButtonBorderPrimary   = 0x005AC8;
    inline constexpr uint32_t ButtonBorderSecondary = 0x404048;

    inline constexpr uint32_t TextPrimary  = 0xF5F5F7;
    inline constexpr uint32_t TextMuted    = 0x8E8E93;

    inline constexpr uint32_t Accent       = 0xFF9500;
    inline constexpr uint32_t Danger       = 0xDC1E1E;  // TRUE RED - no purple
}
