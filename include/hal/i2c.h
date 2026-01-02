/**
 * @file i2c.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>

namespace I2C {
    struct controller_config_t {

    };

    class Controller {
    private:
        static constexpr const char* TAG = "HAL:ADC";

        config_t config;
        bool is_configured;

        std::vector<> registered_channels;

    private:
        explicit Controller();
        ~Controller();

    public:
        /**
         * @brief Returns a reference to the static controller instance
         * @note The instance will be created on the first call of this function
         *
         * @return Reference to the controller object
         */
        static Controller& get_instance() {
            static Controller _instance;
            return _instance;
        }
    };
}