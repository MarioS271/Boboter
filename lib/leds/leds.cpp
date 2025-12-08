/**
 * @file leds.cpp
 * @authors MarioS271
 */

#include "leds.hpp"

#include "predef_Colors.hpp"
#include "logger.hpp"
#include "error.hpp"

Leds::Leds()
{
    ERROR_CHECK(TAG, gpio_set_direction(MOSI_PIN, GPIO_MODE_OUTPUT));
    ERROR_CHECK(TAG, gpio_set_direction(CLK_PIN, GPIO_MODE_OUTPUT));

    allOff();

    LOGI(TAG, "Initialized LEDs");
}

void Leds::setColor(led_pos_t led_pos, rgb_color_t color)
{
    if (led_pos >= 4)
    {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return;
    }

    leds[led_pos] = color;
    update();
}

void Leds::setOff(led_pos_t led_pos)
{
    if (led_pos >= 4)
    {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return;
    }

    setColor(led_pos, Colors::OFF);
}

void Leds::setAll(rgb_color_t color)
{
    for (int i = 0; i < 4; i++)
        leds[i] = color;
    update();
}

void Leds::allOff()
{
    setAll(Colors::OFF);
}

rgb_color_t Leds::getColor(led_pos_t led_pos)
{
    if (led_pos >= 4)
    {
        LOGW(TAG, "Invalid LED position: %d", led_pos);
        return Colors::OFF;
    }
    return leds[led_pos];
}