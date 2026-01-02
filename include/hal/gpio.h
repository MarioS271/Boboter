/**
 * @file gpio.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <driver/gpio.h>

namespace GPIO {
    using bitmask_t = uint64_t;

    struct pin_config_t {
        gpio_num_t gpio_pin;
        gpio_mode_t mode;
        gpio_pull_mode_t pull_mode = GPIO_FLOATING;
        gpio_int_type_t intr_type = GPIO_INTR_DISABLE;
        bool pull_low_in_deep_sleep = false;
    };

    enum class level_t : uint8_t {
        LOW = 0,
        HIGH = 1
    };

    class Controller {
    private:
        static constexpr const char* TAG = "HAL:GPIO";

        struct saved_config_entry_t {
            gpio_num_t gpio_pin;
            gpio_mode_t mode;
        };

        std::vector<saved_config_entry_t> registered_entries;
        bitmask_t pins_to_pull_low;

    private:
        explicit Controller();
        ~Controller();

    public:
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
         * @brief Removes and resets a pin from the microcontroller
         *
         * @param gpio_pin The GPIO pin to remove
         */
        void remove(gpio_num_t gpio_pin);

        /**
         * @brief Checks if a specific pin is already registered
         *
         * @return Whether the pin is registered (true) or not (false)
         *
         * @param gpio_pin The GPIO pin to check for
         */
        [[nodiscard]] bool is_registered(gpio_num_t gpio_pin) const;

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

        /**
         * @brief Pulls low all pins in the @c pull_low_in_deep_sleep bitmask
         */
        void prepare_for_deep_sleep() const;
    };
}