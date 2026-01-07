/**
 * @file leds_helpers.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "leds.h"

#include "include/robot.h"

namespace Device {
    void Leds::send_bit(const bool bit) const {
        using enum HAL::GPIO::level_t;

        robot.gpio.set_level(MOSI_PIN, bit ? HIGH : LOW);
        robot.gpio.set_level(SCK_PIN, HIGH);
        robot.gpio.set_level(SCK_PIN, LOW);
    }

    void Leds::send_byte(const uint8_t byte) const {
        for (int8_t i = 7; i >= 0; --i) {
            send_bit(byte & 1 << i);
        }
    }

    void Leds::send_frame(const rgb_color_t color) const {
        // This sets maximum brightness, as we don't need to regulate it (brightness can already be
        // done through the RGB color value)
        // Equivalent to (0xE0 | 0x1F); 0xE0 is the first three marker bits for frame start,
        // 0x1F is last five bits for maximum brightness
        send_byte(0xFF);

        send_byte(color.r);
        send_byte(color.b);
        send_byte(color.g);
    }

    void Leds::update() const {
        for (int i = 0; i < 4; ++i) send_byte(0x00);
        for (int i = 0; i < NUM_LEDS; ++i) send_frame(leds[i]);
        for (int i = 0; i < 4; ++i) send_byte(0xFF);
    }
}