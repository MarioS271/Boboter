/**
 * @file rgb_color.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once
#include <cstdint>

/**
 * @brief A type for storing a 16-bit RGB color
 */
struct rgb_color_t {
    uint16_t r;     ///< Red Color Value
    uint16_t g;     ///< Green Color Value
    uint16_t b;     ///< Blue Color Value
};