// LEDS_HELPERS_CPP
// Boboter
// (C) MarioS271 2025

#include "leds.h"

void Leds::sendBit(bool bit) {
    gpio_set_level(MOSI_PIN, bit ? 1 : 0);
    gpio_set_level(CLK_PIN, 1);
    gpio_set_level(CLK_PIN, 0);
}

void Leds::sendByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        sendBit(byte & (1 << i));
    }
}

void Leds::sendFrame(rgb_color_t color) {
    sendByte(0b11100000 | (31 & 0x1F)); // 5-bit global brightness
    sendByte(color.r);
    sendByte(color.b);
    sendByte(color.g);
}

void Leds::update() {
    for (int i = 0; i < 4; i++) sendByte(0x00);
    for (int i = 0; i < 4; i++) sendFrame(leds[i]);
    for (int i = 0; i < 4; i++) sendByte(0xFF);
}