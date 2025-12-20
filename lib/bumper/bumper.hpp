/**
 * @file bumper.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "bumper_types.hpp"

#include <driver/gpio.h>

namespace Boboter::Libs::Bumper {
    namespace Config {
        constexpr gpio_num_t LEFT_BUMPER_PIN = GPIO_NUM_35;
        constexpr gpio_num_t RIGHT_BUMPER_PIN = GPIO_NUM_34;
    }

    class Bumper {
    private:
        static constexpr const char* TAG = "Libs::Bumper";
    
        Boboter::Types::Bumper::Id bumper_id;
        gpio_num_t bumper_pin;
    
    public:
        explicit Bumper(Boboter::Types::Bumper::Id bumper_id);
        ~Bumper() = default;
    
        /**
         * @brief Checks if the bumper is hit
         *
         * @return (bool) The state of the bumper (false -> not hit, true -> hit)
        */
        bool isHit() const { return !gpio_get_level(bumper_pin); }
    };
}