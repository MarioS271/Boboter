/**
 * @file motor_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::Motor {
    enum Id : uint8_t {
        LEFT = 0,
        RIGHT = 1
    };
    
    enum Direction : uint8_t {
        FORWARD = 0,
        BACKWARD = 1
    };
}