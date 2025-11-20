// IO_SHIELD_BUTTON_CPP
// Boboter
// (C) MarioS271 2025

#include "io_shield.h"

#include <freertos/task.h>

bool IOShield::getButtonState() {
    bool state = gpio_get_level(BUTTON_PIN) == 0;
    return state;
}

bool IOShield::getDebouncedButtonState() {
    static uint32_t last_time = 0;
    static bool last_state = false;
    bool state = gpio_get_level(BUTTON_PIN) == 0;
    uint32_t now = xTaskGetTickCount();

    if (state != last_state && (now - last_time) > pdMS_TO_TICKS(BUTTON_DEBOUNCE)) {
        last_state = state;
        last_time = now;
        return state;
    }

    return last_state;
}
