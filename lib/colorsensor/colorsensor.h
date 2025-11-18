// COLORSENSOR_H
// Boboter
// (C) MarioS271 2025#

#pragma once

#include <cstdint>
#include "driver/gpio.h"

class Colorsensor {
private:
    static constexpr char* TAG = "COLORSENSOR";
    
    static constexpr gpio_num_t LED_PIN = GPIO_NUM_13;
    

public:
    Colorsensor();
    ~Colorsensor() = default;

    void setLedState(bool state);
};