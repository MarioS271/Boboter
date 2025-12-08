/**
 * @file bumper.hpp
 * @authors MarioS271
 */

#pragma once

#include "bumper_types.hpp"
#include <driver/gpio.h>

class Bumper
{
private:
    static constexpr const char* TAG = "Bumper";
    
    static constexpr gpio_num_t LEFT_BUMPER_PIN = GPIO_NUM_35;
    static constexpr gpio_num_t RIGHT_BUMPER_PIN = GPIO_NUM_34;

    bumper_num_t bumper_num;
    gpio_num_t bumper_pin;
    bool error;

public:
    explicit Bumper(bumper_num_t bumper_number);
    ~Bumper() = default;

    bool isHit() const;
};