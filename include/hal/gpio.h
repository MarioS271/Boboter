/**
 * @file gpio.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief A namespace containing all components of the GPIO hardware abstraction layer
 */
namespace HAL::GPIO {
    using bitmask_t = uint64_t;

    struct pin_config_t {
        gpio_num_t gpio_pin;
        gpio_mode_t mode;
        gpio_pull_mode_t pull_mode;
        gpio_int_type_t intr_type;
    };

    enum class level_t : uint8_t {
        LOW = 0,
        HIGH = 1
    };

    /**
     * @brief The GPIO hardware abstraction layer's control class
     */
    class Controller {
    private:
        static constexpr const char* TAG = "HAL::GPIO";

        mutable SemaphoreHandle_t mutex;

        struct saved_config_entry_t {
            gpio_num_t gpio_pin;
            gpio_mode_t mode;
        };

        std::vector<saved_config_entry_t> registered_entries;

    private:
        explicit Controller();
        ~Controller();

    public:
        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

        /**
         * @brief Gets the controller instance
         * @note The instance will be created on the first call of this function
         *
         * @return Reference to the controller object
         */
        static Controller& get_instance() {
            static Controller _instance;
            return _instance;
        }

        /**
         * @brief Resets all pins and shuts down the controller
         */
        void shutdown();

        /**
         * @brief Configures a new GPIO pin using the given config
         *
         * @param entry The GPIO config entry to configure
         */
        void add(const pin_config_t& entry);

        /**
         * @brief Sets the level of the given GPIO pin
         *
         * @param gpio_pin The pin of which to set the level
         * @param level The level to set the given pin to
         */
        void set_level(gpio_num_t gpio_pin, level_t level) const;

        /**
         * @brief Gets the level of the given GPIO pin
         *
         * @return The current level of the given pin
         *
         * @param gpio_pin The pin of which to get the level
         */
        [[nodiscard]] level_t get_level(gpio_num_t gpio_pin) const;
    };
}