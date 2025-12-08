/**
 * @file constants.hpp
 * @authors MarioS271
 */

#pragma once

#include <cstdint>

namespace Main
{
    constexpr const char* TAG = "Main";

    constexpr uint32_t TASK_STACK_DEPTH = 4096;

    constexpr uint8_t LED_TASK_PRIORITY = 1;
    constexpr uint8_t WEBUI_TASK_PRIORITY = 2;
    constexpr uint8_t IO_SHIELD_TASK_PRIORITY = 3;
    constexpr uint8_t SENSOR_TEST_TASK_PRIORITY = 9;
}