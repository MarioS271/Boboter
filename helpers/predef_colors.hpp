/**
 * @file predef_colors.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>
#include "types/rgb_color.hpp"

namespace Boboter::Helpers::Colors {
    constexpr uint8_t NUM_COLORS = 8;  // Not including OFF

    constexpr rgb_color_t OFF = {0, 0, 0};

    constexpr rgb_color_t RED = {255, 0, 0};
    constexpr rgb_color_t ORANGE = {255, 160, 0};
    constexpr rgb_color_t YELLOW = {255, 255, 0};
    constexpr rgb_color_t GREEN = {0, 255, 0};
    constexpr rgb_color_t CYAN = {0, 255, 255};
    constexpr rgb_color_t BLUE = {0, 0, 255};
    constexpr rgb_color_t MAGENTA = {255, 0, 255};
    constexpr rgb_color_t WHITE = {255, 255, 255};
}