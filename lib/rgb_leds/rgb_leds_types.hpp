/**
 * @file leds_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::RgbLeds {
    enum Id : uint8_t {
        LED_FRONT_LEFT = 0,
        LED_FRONT_RIGHT = 1,
        LED_BACK_LEFT = 2,
        LED_BACK_RIGHT = 3
    };
}