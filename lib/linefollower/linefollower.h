// LINEFOLLOWER_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "linefollower_types.h"
#include "driver/gpio.h"

class Linefollower {
private:
    static constexpr gpio_num_t LF_RIGHT_LEFT_PIN = GPIO_NUM_12;
    static constexpr gpio_num_t LINEFOLLOWER_PIN = GPIO_NUM_36;

public:
    Linefollower();

    lf_result_t get(lf_module_t module);
};