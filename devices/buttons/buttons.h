/**
 * @file buttons.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Buttons {
    private:
        static constexpr const char* TAG = "Device::Buttons";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_BUTTONS;

        static constexpr gpio_num_t BUTTON_PRIMARY_PIN = GPIO_NUM_16;
        static constexpr gpio_num_t BUTTON_SECONDARY_PIN = GPIO_NUM_17;
        static constexpr uint32_t DEBOUNCE_US = 250'000;

        Robot& robot;
        bool primary_button_state;
        bool secondary_button_state;

    public:
        explicit Buttons(Robot& robot);
        ~Buttons();

        /**
         * @brief Sets up the necessary GPIO pins
         */
        void initialize();

        /**
         * @brief Updates the button states with the current hardware states
         */
        void update_button_states();

        /**
         * @brief Gets the state of the primary button
         */
        [[nodiscard]] bool get_button_state_primary() const { return primary_button_state; }

        /**
         * @brief Gets the state of the secondary button
         */
        [[nodiscard]] bool get_button_state_secondary() const { return secondary_button_state; }
    };
}