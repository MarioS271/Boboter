// LINEFOLLOWER_CPP
// Boboter
// (C) MarioS271 2025

#include "linefollower.h"
#include "linefollower_types.h"
#include "logger.h"

Linefollower::Linefollower() {
    gpio_set_direction(LF_RIGHT_LEFT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LF_RIGHT_LEFT_PIN, LF_RIGHT);
}

lf_result_t Linefollower::get(lf_module_t module) {
    gpio_set_level(LF_RIGHT_LEFT_PIN, module);
    return LF_WHITE;
}