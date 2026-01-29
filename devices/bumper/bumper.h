/**
 * @file bumper.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>

class Robot;

namespace Device {
    class Bumper {
    private:
        static constexpr const char* TAG = "Device::Bumpers";

        static constexpr gpio_num_t LEFT_PIN = GPIO_NUM_35;
        static constexpr gpio_num_t RIGHT_PIN = GPIO_NUM_34;

        Robot& robot;

    public:
        enum class bumper_id_t : uint8_t {
            LEFT = 0,
            RIGHT = 1
        };

    public:
        explicit Bumper(Robot& robot);
        ~Bumper();

        /**
         * @brief Sets up the necessary GPIO pins
         */
        void initialize();

        /**
         * @brief Checks if the bumper is hit
         *
         * @return (bool) The state of the bumper (false -> not hit, true -> hit)
        */
        bool is_hit(bumper_id_t bumper_id) const;
    };
}