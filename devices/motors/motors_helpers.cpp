/**
 * @file motors_helpers.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "motors.h"

namespace Device {
    Motors::motor_direction_t Motors::get_actual_direction(motor_id_t motor_id, motor_direction_t apparent_direction) const {
        const auto motor_id_as_int = static_cast<uint8_t>(motor_id);

        return static_cast<motor_direction_t>(
            static_cast<uint8_t>(apparent_direction)
            xor
            static_cast<uint8_t>(inverse_direction[motor_id_as_int])
        );
    }
}
