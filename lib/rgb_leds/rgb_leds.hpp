/**
 * @file leds.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "rgb_leds_types.hpp"

#include <driver/gpio.h>
#include "types/rgb_color.hpp"

namespace Boboter::Libs::RGB_Leds {
    namespace Config {
        constexpr gpio_num_t MOSI_PIN = GPIO_NUM_23;
        constexpr gpio_num_t CLK_PIN = GPIO_NUM_18;
        constexpr uint8_t NUM_LEDS = 4;
    }

    class RGB_Leds {
    private:
        static constexpr const char* TAG = "Libs::RGB_Leds";
    
        void sendBit(bool bit);
        void sendByte(uint8_t byte);
        void sendFrame(Boboter::Types::rgb_color_t color);
        void update();

        Boboter::Types::rgb_color_t leds[Config::NUM_LEDS] = {};
    
    public:
        explicit RGB_Leds();
        ~RGB_Leds() = default;
    
        /**
         * @brief Sets the color of one LED
         *
         * @return (void)  
         *
         * @param led_id The id of the LED of which to set the color
         * @param color The color to set the LED to
        */
        void setColor(Boboter::Types::RGB_Leds::Id led_id, Boboter::Types::rgb_color_t color);

        /**
         * @brief Sets the color of all LEDs
         *
         * @return (void)  
         *
         * @param color The color to set the LEDs to
        */
        void setAll(Boboter::Types::rgb_color_t color);

        /**
         * @brief Turns off one LED
         *
         * @return (void)  
         *
         * @param led_id The id of the LED to turn off
        */
        void setOff(Boboter::Types::RGB_Leds::Id led_id);

        /**
         * @brief Turns off all LEDs
         *
         * @return (void)  
        */
        void allOff();
    
        /**
         * @brief Gets the currently set color of one LED
         *
         * @return (rgb_color_t) The color of the LED
         *
         * @param led_id The id of the LED of which to fetch the color
        */
        Boboter::Types::rgb_color_t getColor(Boboter::Types::RGB_Leds::Id led_id);
    };
}