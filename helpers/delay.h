/**
 * @file delay.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <esp_timer.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief Wrapper for vTaskDelay(pdMS_TO_TICKS(ms))
 */
inline void delay(const uint16_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}