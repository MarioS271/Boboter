/**
 * @file leds_types.hpp
 * @authors MarioS271
 */

#pragma once

#include <cstdint>

enum led_pos_t : uint8_t
{
    LED_FRONT_LEFT = 0,
    LED_FRONT_RIGHT = 1,
    LED_BACK_LEFT = 2,
    LED_BACK_RIGHT = 3
};