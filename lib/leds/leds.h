// LEDS_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/gpio.h"
#include "hex_color.h"

enum led_pos_t : uint8_t {
    LED_FRONT_LEFT = 0,
    LED_FRONT_RIGHT = 1,
    LED_BACK_LEFT = 2,
    LED_BACK_RIGHT = 3
};

class Leds {
private:
    static constexpr gpio_num_t MOSI_PIN = GPIO_NUM_23;
    static constexpr gpio_num_t CLK_PIN = GPIO_NUM_18;

public:
    explicit Leds();

    void setColor(led_pos_t led_pos, hex_color_t color);
};