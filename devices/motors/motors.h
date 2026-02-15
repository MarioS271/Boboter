/**
 * @file motors.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>
#include <driver/ledc.h>
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Motors {
    private:
        static constexpr const char* TAG = "Device::Motors";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_MOTORS;

        static constexpr gpio_num_t LEFT_SPEED_PIN = GPIO_NUM_32;
        static constexpr gpio_num_t LEFT_DIRECTION_PIN = GPIO_NUM_33;
        static constexpr gpio_num_t RIGHT_SPEED_PIN = GPIO_NUM_2;
        static constexpr gpio_num_t RIGHT_DIRECTION_PIN = GPIO_NUM_15;

        static constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
        static constexpr ledc_channel_t LEFT_LEDC_CHANNEL = LEDC_CHANNEL_0;
        static constexpr ledc_channel_t RIGHT_LEDC_CHANNEL = LEDC_CHANNEL_1;

        Robot& robot;
        bool inverse_direction[2];

    public:
        static constexpr uint8_t NUM_MOTORS = 2;
        static constexpr uint16_t MAX_MOTOR_SPEED = 1023;
        static constexpr uint16_t DEFAULT_RAMP_TIME_MS = 1000;
        static constexpr uint16_t MIN_RAMP_TIME_MS = 100;

        enum class motor_id_t : uint8_t {
            LEFT = 0,
            RIGHT = 1
        };

        enum class motor_direction_t : uint8_t {
            FORWARD = 0,
            BACKWARD = 1,
        };

    public:
        explicit Motors(Robot& robot);
        ~Motors();

        /**
         * @brief Sets up the necessary GPIO pins and LEDC channels
         */
        void initialize();

    // Wrapper Functions
        /**
         * @brief Stops the given motor by gradually reducing speed for a smooth stop
         *
         * @param motor_id The id of the motor to stop
         * @param ramp_time The time to take when ramping to that speed (default: @c DEFAULT_RAMP_TIME_MS)
         */
        void stop(motor_id_t motor_id, uint16_t ramp_time = DEFAULT_RAMP_TIME_MS) const;

        /**
         * @brief Stops the given motor abruptly without ramping
         *
         * @param motor_id The id of the motor to stop
         */
        void hard_stop(motor_id_t motor_id) const;

        /**
         * @brief Sets the speed of the given motor toward which it should accelerate
         * @note The motor will immediately begin accelerating towards the specified speed
         *
         * @param motor_id The id of the motor of which to set the speed
         * @param speed The target speed to set the motor to
         * @param ramp_time The time to take when ramping to that speed (default: @c DEFAULT_RAMP_TIME_MS)
         */
        void set_speed(motor_id_t motor_id, uint16_t speed, uint16_t ramp_time = DEFAULT_RAMP_TIME_MS) const;

        /**
         * @brief Sets the virtual direction of the given motor
         *
         * @param motor_id The id of the motor of which to set the direction
         * @param direction The target direction to set the motor to
         * @param ramp_time The time to take when ramping to that speed (default: @c DEFAULT_RAMP_TIME_MS)
         */
        void set_direction(motor_id_t motor_id, motor_direction_t direction, uint16_t ramp_time = DEFAULT_RAMP_TIME_MS) const;

    // Hardware Functions
        /**
         * @brief Physically sets the speed of the given motor
         * @note The motor will immediately begin turning at that exact speed
         *
         * @param motor_id The id of the motor of which to set the speed
         * @param speed The target speed to set the motor to
         */
        void _set_speed(motor_id_t motor_id, uint16_t speed) const;

        /**
         * @brief Physically sets the direction of the given motor
         *
         * @param motor_id The id of the motor of which to set the direction
         * @param direction The target direction to set the motor to
         */
        void _set_direction(motor_id_t motor_id, motor_direction_t direction) const;

        /**
         * @brief Gets the true physical motor direction which is computed using
         *        the @c inverse_direction modifier
         *
         * @return The direction the wheel will actually spin
         *
         * @param motor_id The id of the motor to determine the true direction of
         * @param apparent_direction The apparent direction to process
         */
        [[nodiscard]] motor_direction_t _get_actual_direction(motor_id_t motor_id, motor_direction_t apparent_direction) const;
    };
}