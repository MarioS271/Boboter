/**
 * @file leds_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::RGB_Leds {
    enum Id : uint8_t {
        FRONT_LEFT = 0,
        FRONT_RIGHT = 1,
        BACK_LEFT = 2,
        BACK_RIGHT = 3
    };
}