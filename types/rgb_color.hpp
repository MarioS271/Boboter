/**
 * @file rgb_color.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types {
    /**
     * @struct RGB_Color
     * @brief Type to represent an RGB color with 16-bit depth per channel
    */
    struct rgb_color_t {
        uint16_t r;
        uint16_t g;
        uint16_t b;
    };
}