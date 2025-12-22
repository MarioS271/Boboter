/**
 * @file constants.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Main::Constants {
    constexpr const char* TAG = "Main";

    constexpr uint8_t SECURE_TASK_PRIORITY = 24;
    constexpr uint16_t SECURE_TASK_STACK_DEPTH = 1024;
    
    constexpr uint8_t SYSTEM_TASK_PRIORITY = 7;
    constexpr uint16_t SYSTEM_TASK_STACK_DEPTH = 2048;

    constexpr uint8_t PID_TASK_PRIORITY = 15;
    constexpr uint16_t PID_TASK_STACK_DEPTH = 4096;

    constexpr uint8_t IO_TASK_PRIORITY = 10;
    constexpr uint16_t IO_TASK_STACK_DEPTH = 2048;

    constexpr uint8_t BLUEPAD_TASK_PRIORITY = 8;
    constexpr uint16_t BLUEPAD_TASK_STACK_DEPTH = 4096;

    constexpr uint8_t LEDS_TASK_PRIORITY = 5;
    constexpr uint16_t LEDS_TASK_STACK_DEPTH = 2048;

    constexpr uint8_t BUZZER_TASK_PRIORITY = 1;
    constexpr uint16_t BUZZER_TASK_STACK_DEPTH = 1024;

    constexpr uint8_t SENSOR_TEST_TASK_PRIORITY = 22;
    constexpr uint16_t SENSOR_TEST_TASK_STACK_DEPTH = 8192;
}