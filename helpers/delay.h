// DELAY_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "freertos/FreeRTOS.h"

inline void delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}