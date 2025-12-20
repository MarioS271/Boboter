/**
 * @file other_leds.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <driver/gpio.h>

namespace Boboter::Libs::OtherLeds {
    namespace Config {
        constexpr gpio_num_t STATUS_LED_PIN = GPIO_NUM_5;
        constexpr gpio_num_t BOTTOM_LED_PIN = GPIO_NUM_13;
    }

    class OtherLeds {
    private:
        static constexpr const char* TAG = "Libs::OtherLeds";

        bool status_led_state;
        bool bottom_led_state;

    public:
        explicit OtherLeds();
        ~OtherLeds() = default;

        /**
         * @brief Sets the state of the status LED
         *
         * @return (void)  
         *
         * @param new_state The new state to set the LED to
        */
        void setStatusLed(bool new_state);

        /**
         * @brief Sets the state of the bottom LED
         *
         * @return (void)  
         *
         * @param new_state The new state to set the LED to
        */
        void setBottomLed(bool new_state);

        /**
         * @brief Gets the current state of the status LED
         *
         * @return (bool) The state of the LED
        */
        bool getStatusLed() const { return status_led_state; }

        /**
         * @brief Gets the current state of the bottom LED
         *
         * @return (bool) The state of the LED
        */
        bool getBottomLed() const { return bottom_led_state; }
    };
}