// DELAY_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

inline void delay(unsigned int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}