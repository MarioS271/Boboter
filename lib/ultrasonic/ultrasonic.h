// ULTRASONIC_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "driver/gpio.h"

class Ultrasonic {
private:
    static constexpr gpio_num_t TRIGGER_PIN = GPIO_NUM_25;
    static constexpr gpio_num_t ECHO_PIN = GPIO_NUM_26;

public:
    Ultrasonic();
    ~Ultrasonic() = default;
    
    float measureCm();
};
