/**
 * @file ledc.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief A namespace containing all components of the LEDC hardware abstraction layer
 */
namespace LEDC {
    struct timer_config_t {
        ledc_timer_t timer;
        uint32_t frequency = 10'000;
        ledc_timer_bit_t resolution = LEDC_TIMER_10_BIT;
        ledc_mode_t speed_mode = LEDC_LOW_SPEED_MODE;
    };

    struct channel_config_t {
        ledc_channel_t channel;
        ledc_timer_t timer;
        gpio_num_t gpio_pin;
        uint32_t duty = 0;
    };

    /**
     * @brief The LEDC hardware abstraction layer's control class
     */
    class Controller {
    private:
        static constexpr const char* TAG = "HAL:LEDC";

        SemaphoreHandle_t mutex;

        struct saved_channel_config_t {
            ledc_channel_t channel;
            gpio_num_t gpio_pin;
        };

        std::vector<ledc_timer_t> registered_timers;
        std::vector<saved_channel_config_t> registered_channels;

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
         * @brief Resets all channels and shuts down the controller
         */
        void shutdown();

        /**
         * @brief Configures a new LEDC timer using the given config
         *
         * @param config The timer config to use
         */
        void add_timer(const timer_config_t& config);

        /**
         * @brief Configures a new LEDC channel using the given config
         *
         * @param config The channel config to use
         */
        void add_channel(const channel_config_t& config);

        /**
         * @brief Removes and resets a pin from the microcontroller
         *
         * @param ledc_channel The channel to remove
         */
        void remove(ledc_channel_t ledc_channel);

        /**
         * @brief Checks if a specific channel is already registered
         *
         * @return Whether the channel is registered (true) or not (false)
         *
         * @param ledc_channel The channel to check for
         */
        [[nodiscard]] bool is_registered(ledc_channel_t ledc_channel) const;

        /**
         * @brief Sets the frequency of a specific LEDC timer
         *
         * @param ledc_timer The timer of which to set the frequency
         * @param frequency The target frequency
         */
        void set_frequency(ledc_timer_t ledc_timer, uint32_t frequency);

        /**
         * @brief Sets the duty cycle of a specific LEDC channel
         *
         * @param ledc_channel The channel of which to set the duty cycle
         * @param duty The target duty cycle
         */
        void set_duty(ledc_channel_t ledc_channel, uint32_t duty);

        /**
         * @brief Pulls low all pins of which channels are registered
         */
        void prepare_for_deep_sleep();
    };
}