// LEDS_CPP
// Boboter
// (C) MarioS271 2025

#include "leds.h"

#include "predef_colors.h"
#include "logger.h"
#include "error.h"

Leds::Leds() {
    ERROR_CHECK(TAG, gpio_set_direction(MOSI_PIN, GPIO_MODE_OUTPUT));
    ERROR_CHECK(TAG, gpio_set_direction(CLK_PIN, GPIO_MODE_OUTPUT));

    allOff();

    LOGI(TAG, "Initialized LEDs");
}

void Leds::setColor(led_pos_t led_pos, rgb_color_t color) {
    if (led_pos >= 4) {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return;
    }

    leds[led_pos] = color;
    update();
}

void Leds::setOff(led_pos_t led_pos) {
    if (led_pos >= 4) {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return;
    }

    setColor(led_pos, COLORS::OFF);
}

void Leds::setAll(rgb_color_t color) {
    for (int i = 0; i < 4; i++) leds[i] = color;
    update();
}

void Leds::allOff() {
    setAll(COLORS::OFF);
}

rgb_color_t Leds::getColor(led_pos_t led_pos) {
    if (led_pos >= 4) {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return COLORS::OFF;
    }
    return leds[led_pos];
}