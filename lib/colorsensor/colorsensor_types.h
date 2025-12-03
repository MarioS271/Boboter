// COLORSENSOR_TYPES_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>

struct RGBC {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t c;
};

enum class IntegrationTime : uint8_t {
    MS_2_4 = 0xFF,   // 2.4ms
    MS_24 = 0xF6,    // 24ms
    MS_50 = 0xEB,    // 50ms
    MS_101 = 0xD5,   // 101ms
    MS_154 = 0xC0,   // 154ms
    MS_700 = 0x00    // 700ms
};

enum class Gain : uint8_t {
    GAIN_1X  = 0x00,
    GAIN_4X  = 0x01,
    GAIN_16X = 0x02,
    GAIN_60X = 0x03
};