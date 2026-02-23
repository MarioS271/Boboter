/**
 * @file predef_colors.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include "types/rgb_color.h"

/**
 * @brief A namespace containing predefined @c rgb_color_t values
 */
namespace Colors {
    constexpr uint8_t NUM_COLORS = 8;              /// Not including @c Colors::BLACK
    constexpr uint8_t NUM_COLORS_WITH_BLACK = 9;   /// Including @c Colors::BLACK

    constexpr rgb_color_t BLACK = {0, 0, 0};
    constexpr rgb_color_t RED = {255, 0, 0};
    constexpr rgb_color_t ORANGE = {255, 160, 0};
    constexpr rgb_color_t YELLOW = {255, 255, 0};
    constexpr rgb_color_t GREEN = {0, 255, 0};
    constexpr rgb_color_t CYAN = {0, 255, 255};
    constexpr rgb_color_t BLUE = {0, 0, 255};
    constexpr rgb_color_t MAGENTA = {255, 0, 255};
    constexpr rgb_color_t WHITE = {255, 255, 255};

    /**
     * @brief An array containing all the colors to make it easy to loop over them or randomly pick one
     * @attention This does NOT contain @c Colors::BLACK. If you need one that does, use @c Colors::LIST_WITH_BLACK instead
     */
    constexpr rgb_color_t LIST[NUM_COLORS] = {
        RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE
    };

    /**
     * @brief An array containing all the colors to make it easy to loop over them or randomly pick one
     * @attention This does contain @c Colors::BLACK. If you need one that doesn't, use @c Colors::LIST instead
     */
    constexpr rgb_color_t LIST_WITH_BLACK[NUM_COLORS + 1] = {
        BLACK, RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE
    };
}