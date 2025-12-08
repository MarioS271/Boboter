/**
 * @file leds.hpp
 * @authors MarioS271
 */

#pragma once

#include "leds_types.hpp"

#include <driver/gpio.h>
#include "rgb_color.hpp"

class Leds
{
private:
    static constexpr const char* TAG = "Leds";
    
    static constexpr gpio_num_t MOSI_PIN = GPIO_NUM_23;
    static constexpr gpio_num_t CLK_PIN = GPIO_NUM_18;

    void sendBit(bool bit);
    void sendByte(uint8_t byte);
    void sendFrame(rgb_color_t color);
    void update();

    rgb_color_t leds[4] = {};

public:
    Leds();
    ~Leds() = default;

    void setColor(led_pos_t led_pos, rgb_color_t color);
    void setAll(rgb_color_t color);
    void setOff(led_pos_t led_pos);
    void allOff();

    rgb_color_t getColor(led_pos_t led_pos);
};