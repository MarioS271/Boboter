// LEDS_CPP
// Boboter
// (C) MarioS271 2025

#include "leds.h"

#include "logger.h"
#include "predef_colors.h"

#define TAG "LEDS"

Leds::Leds() {
    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = (1ULL << MOSI_PIN) | (1ULL << CLK_PIN);
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_conf);

    LOGI(TAG, "LEDs initialized on MOSI: %d, CLK: %d", MOSI_PIN, CLK_PIN);

    allOff();
}

Leds::~Leds() {
    allOff();
}

void Leds::setColor(led_pos_t led_pos, rgb_color_t color) {
    if (led_pos >= 4) {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return;
    }
    leds[led_pos] = color;
    LOGD(TAG, "LED %d set to R=%d G=%d B=%d", led_pos, color.r, color.g, color.b);
    update();
}

void Leds::setAll(rgb_color_t color) {
    for (int i = 0; i < 4; i++) leds[i] = color;
    LOGD(TAG, "All LEDs set to R=%d G=%d B=%d", color.r, color.g, color.b);
    update();
}

void Leds::setOff(led_pos_t led_pos) {
    LOGD(TAG, "Turning LED %d off", led_pos);
    setColor(led_pos, COLORS::OFF);
}

void Leds::allOff() {
    LOGD(TAG, "Turning all LEDs off");
    setAll(COLORS::OFF);
}

rgb_color_t Leds::getColor(led_pos_t led_pos) {
    if (led_pos >= 4) {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return COLORS::OFF;
    }
    return leds[led_pos];
}