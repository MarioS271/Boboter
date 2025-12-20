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
        MOTOR_LEFT = 0,
        MOTOR_RIGHT = 1
    };
    
    enum Direction : uint8_t {
        M_FORWARD = 0,
        M_BACKWARD = 1
    };
}