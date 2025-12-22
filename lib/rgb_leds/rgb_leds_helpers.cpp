/**
 * @file leds_helpers.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "rgb_leds.hpp"

namespace Boboter::Libs::RGB_Leds {
    void RGB_Leds::send_bit(bool bit) {
        using namespace Config;

        gpio_set_level(MOSI_PIN, bit ? 1 : 0);
        gpio_set_level(CLK_PIN, 1);
        gpio_set_level(CLK_PIN, 0);
    }

    void RGB_Leds::send_byte(uint8_t byte) {
        for (int8_t i = 7; i >= 0; i--)
            send_bit(byte & (1 << i));
    }

    void RGB_Leds::send_frame(Boboter::Types::rgb_color_t color) {
        send_byte(0b11100000 | (31 & 0x1F)); // 5-bit global brightness
        send_byte(color.r);
        send_byte(color.b);
        send_byte(color.g);
    }

    void RGB_Leds::update() {
        for (int i = 0; i < 4; i++) send_byte(0x00);
        for (int i = 0; i < 4; i++) send_frame(leds[i]);
        for (int i = 0; i < 4; i++) send_byte(0xFF);
    }
}