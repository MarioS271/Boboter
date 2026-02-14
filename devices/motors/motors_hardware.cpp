/**
 * @file motors_hardware.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "motors.h"

#include <algorithm>
#include "include/robot/robot.h"

namespace Device {
    void Motors::_set_speed(const motor_id_t motor_id, const uint16_t speed) const {
        using enum motor_id_t;

        robot.ledc.set_duty(
            motor_id == LEFT ? LEFT_LEDC_CHANNEL : RIGHT_LEDC_CHANNEL,
            std::clamp<uint16_t>(speed, 0, MAX_MOTOR_SPEED)
        );

        LOGV("Set %s motor to speed %hd", motor_id == LEFT ? "LEFT" : "RIGHT", speed);
    }

    void Motors::_set_direction(const motor_id_t motor_id, const motor_direction_t direction) const {
        using enum motor_id_t;
        using enum motor_direction_t;
        using enum HAL::GPIO::level_t;

        robot.gpio.set_level(
            motor_id == LEFT ? LEFT_DIRECTION_PIN : RIGHT_DIRECTION_PIN,
            _get_actual_direction(motor_id, direction) == FORWARD ? HIGH : LOW
        );

        LOGV("Set %s motor to direction %s", motor_id == LEFT ? "LEFT" : "RIGHT", direction == FORWARD ? "FORWARD" : "BACKWARD");

    }

    Motors::motor_direction_t Motors::_get_actual_direction(motor_id_t motor_id, motor_direction_t apparent_direction) const {
        const auto motor_id_as_int = static_cast<uint8_t>(motor_id);

        return static_cast<motor_direction_t>(
            static_cast<uint8_t>(apparent_direction)
            xor
            static_cast<uint8_t>(inverse_direction[motor_id_as_int])
        );
    }
}