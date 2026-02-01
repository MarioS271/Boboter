/**
 * @file motors.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>

class Robot;

namespace Device {
    class Motors {
    private:
        static constexpr const char* TAG = "Device::Motors";

        static constexpr gpio_num_t LEFT_SPEED_PIN = GPIO_NUM_32;
        static constexpr gpio_num_t LEFT_DIRECTION_PIN = GPIO_NUM_33;
        static constexpr gpio_num_t RIGHT_SPEED_PIN = GPIO_NUM_2;
        static constexpr gpio_num_t RIGHT_DIRECTION_PIN = GPIO_NUM_15;

        Robot& robot;
        bool inverse_direction[2];

    public:
        static constexpr uint16_t MAX_MOTOR_SPEED = 1023;

        enum class motor_id_t : uint8_t {
            LEFT = 0,
            RIGHT = 1
        };

        enum class motor_direction_t : uint8_t {
            FORWARD = 0,
            BACKWARD = 1,
        };

    private:
        /**
         * @brief Gets the true physical motor direction which is computed using
         *        the @c inverse_direction modifier
         *
         * @return The direction the wheel will actually spin
         *
         * @param motor_id The id of the motor to determine the true direction of
         * @param apparent_direction The apparent direction to process
         */
        motor_direction_t get_actual_direction(motor_id_t motor_id, motor_direction_t apparent_direction) const;

    public:
        explicit Motors(Robot& robot);
        ~Motors();

        /**
         * @brief Sets up the necessary GPIO pins and LEDC channels
         */
        void initialize();

        /**
         * @brief Stops the given motor by gradually reducing speed for a smooth stop
         *
         * @param motor_id The id of the motor to stop
         */
        void stop(motor_id_t motor_id);

        /**
         * @brief Stops the given motor instantly
         *
         * @param motor_id The id of the motor to stop
         */
        void hard_stop(motor_id_t motor_id);

        /**
         * @brief Sets the speed of the given motor toward which it should accelerate
         * @note The motor will immediately begin turning at that speed
         *
         * @param motor_id The id of the motor of which to set the speed
         * @param speed The target speed to set the motor to
         */
        void set_speed(motor_id_t motor_id, uint16_t speed);

        /**
         * @brief Sets the direction of the given motor
         *
         * @param motor_id The id of the motor of which to set the direction
         * @param direction The target direction to set the motor to
         */
        void set_direction(motor_id_t motor_id, motor_direction_t direction);
    };
}