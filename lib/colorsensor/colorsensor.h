// COLORSENSOR_H
// Boboter
// (C) MarioS271 2025#

#pragma once

#include <cstdint>
#include "driver/gpio.h"

class Colorsensor {
private:
    static constexpr gpio_num_t LED_PIN = GPIO_NUM_13;
    

public:
    explicit Colorsensor();

    void setLedState(bool state);
};