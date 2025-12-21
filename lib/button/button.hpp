/**
 * @file button.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "button_types.hpp"

#include <driver/gpio.h>

namespace Boboter::Libs::Button {
    namespace Config {
        constexpr gpio_num_t PRIMARY_PIN = GPIO_NUM_16;
        constexpr gpio_num_t SECONDARY_PIN = GPIO_NUM_17;
        constexpr uint8_t DEBOUNCE_US = 250'000;
    }

    class Button {
    private:
        static constexpr const char* TAG = "Libs::Button";

        Boboter::Types::Button::Id button_id;
        gpio_num_t button_pin;
        uint64_t last_press;

    public:
        explicit Button(Boboter::Types::Button::Id button_id);
        ~Button() = default;

        /**
         * @brief Gets the current state of the button
         *
         * @return (bool) Whether the button is pressed or not (false -> not pressed, true -> pressed)
        */
        bool get_state() const { return !gpio_get_level(button_pin); }

        /**
         * @brief Checks for a debounced button press
         *
         * @return (bool) Whether the button is pressed or not (false -> not pressed, true -> pressed)
        */
        bool get_debounced_press();
    };
}