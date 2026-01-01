/**
 * @file i2c.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

/**
 * @brief A namespace containing everything for the I2C hardware abstraction layer
 */
namespace I2C {
    /**
     * @brief Data structure to hold the information for initializing the I2C controller
     */
    struct config_t {

    };

    /**
     * @brief An I2C hardware abstraction layer controller class
     */
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