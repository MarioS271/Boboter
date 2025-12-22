/**
 * @file secure_task.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Tasks::Secure {
    namespace Config {
        constexpr uint16_t BATTERY_MIN_VOLTAGE_MV = 3'350;
        constexpr uint16_t ALERT_TIME_MS = 1'000;
        constexpr uint16_t SHUTOFF_TIME_MS = 5'000;
    }

    namespace Internal {
        constexpr const char* TAG = "Tasks::Secure";
    }

    void task(void* params);
}