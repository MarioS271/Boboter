/**
 * @file delay.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <esp_timer.h>
#include <freertos/task.h>

namespace Boboter::Helpers {
    /**
     * @brief Wrapper for vTaskDelay(pdMS_TO_TICKS(ms))
     *
     * @return void
     */
    inline void delay(int ms) {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}