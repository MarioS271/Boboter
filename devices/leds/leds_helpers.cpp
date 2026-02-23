/**
 * @file leds_helpers.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "leds.h"

#include "include/robot.h"

namespace Device {
    void Leds::send_bit(const bool bit) const {
        if (bit) {
            *mosi_fast_path.set_register = mosi_fast_path.pin_mask;
        } else {
            *mosi_fast_path.clear_register = mosi_fast_path.pin_mask;
        }

        *sck_fast_path.set_register = sck_fast_path.pin_mask;
        *sck_fast_path.clear_register = sck_fast_path.pin_mask;
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
        //                              0x1F is last five bits for maximum brightness
        send_byte(0xFF);

        send_byte(color.r);
        send_byte(color.b);
        send_byte(color.g);
    }
}