/**
 * @file i2c.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief A namespace containing all components of the I2C hardware abstraction layer
 */
namespace I2C {
    struct controller_config_t {

    };

    /**
     * @brief The I2C hardware abstraction layer's control class
     */
    class Controller {
    private:
        static constexpr const char* TAG = "HAL:ADC";

        SemaphoreHandle_t mutex;

        controller_config_t config;
        bool is_configured;

        std::vector<> registered_xxxxxxxxx;

    private:
        explicit Controller();
        ~Controller();

    public:
        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

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