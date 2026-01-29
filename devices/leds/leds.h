/**
 * @file leds.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>
#include "types/rgb_color.h"

class Robot;

namespace Device {
    class Leds {
    private:
        static constexpr const char* TAG = "Device::Leds";

        static constexpr uint8_t NUM_LEDS = 4;
        static constexpr gpio_num_t MOSI_PIN = GPIO_NUM_23;
        static constexpr gpio_num_t SCK_PIN = GPIO_NUM_18;

        Robot& robot;
        rgb_color_t leds[NUM_LEDS];

    private:
        void send_bit(bool bit) const;              ///< @brief Sends a single bit
        void send_byte(uint8_t byte) const;         ///< @brief Sends an entire byte
        void send_frame(rgb_color_t color) const;   ///< @brief Sends a frame (full color info)

    public:
        enum class led_id_t : uint8_t {
            FRONT_LEFT = 0,
            FRONT_RIGHT = 1,
            BACK_LEFT = 2,
            BACK_RIGHT = 3
        };

    public:
        explicit Leds(Robot& robot);
        ~Leds();

        /**
         * @brief Sets up the necessary GPIO pins
         */
        void initialize();

        /**
         * @brief Prepares the LEDs for a shut-down state, for example for getting
         *        destroyed or entering deep sleep
         */
        void shutdown();

        /**
         * @brief Updates the physical LEDs
         */
        void update() const;

        /**
         * @brief Sets the color of a specific LED
         *
         * @param led_id The ID of the LED of which to set the color
         * @param color The color to set that LED to
         * @param do_update Decides if the function automatically updates the LEDs or not (default: true)
         */
        void set_color(led_id_t led_id, rgb_color_t color, bool do_update = true);

        /**
         * @brief Sets the color of all LEDs
         *
         * @param color The color to set to
         */
        void set_color_all(rgb_color_t color);

        /**
         * @brief Turns off a specific LED
         *
         * @param led_id The ID of the LED to turn off
         * @param do_update Decides if the function automatically updates the LEDs or not (default: true)
         */
        void turn_off(led_id_t led_id, bool do_update = true);

        /**
         * @brief Turns off all LEDs
         */
        void turn_all_off();
    };
}