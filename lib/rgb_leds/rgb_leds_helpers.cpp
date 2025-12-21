/**
 * @file leds_helpers.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "rgb_leds.hpp"

namespace Boboter::Libs::RGB_Leds {
    void RGB_Leds::sendBit(bool bit) {
        using namespace Config;

        gpio_set_level(MOSI_PIN, bit ? 1 : 0);
        gpio_set_level(CLK_PIN, 1);
        gpio_set_level(CLK_PIN, 0);
    }

    void RGB_Leds::sendByte(uint8_t byte) {
        for (uint8_t i = 7; i >= 0; i--)
            sendBit(byte & (1 << i));
    }

    void RGB_Leds::sendFrame(Boboter::Types::rgb_color_t color) {
        sendByte(0b11100000 | (31 & 0x1F)); // 5-bit global brightness
        sendByte(color.r);
        sendByte(color.b);
        sendByte(color.g);
    }

    void RGB_Leds::update() {
        for (int i = 0; i < 4; i++) sendByte(0x00);
        for (int i = 0; i < 4; i++) sendFrame(leds[i]);
        for (int i = 0; i < 4; i++) sendByte(0xFF);
    }
}