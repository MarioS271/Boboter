// MOTORS_TYPES_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>

constexpr uint16_t MAX_MOTOR_SPEED = 1023;

enum motor_num_t : uint8_t {
    MOTOR_LEFT = 0,
    MOTOR_RIGHT = 1
};

enum motor_direction_t : uint8_t {
    M_FORWARD = 0,
    M_BACKWARD = 1
};