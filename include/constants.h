/**
 * @file constants.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <cstdint>

/**
 * @brief A namespace containing contstants for @c app_main
 */
namespace Constants {
    constexpr const char* TAG = "Main";

    constexpr const char* SECURE_TASK_NAME = "SecureTask";
    constexpr uint8_t SECURE_TASK_PRIORITY = 24;
    constexpr uint16_t SECURE_TASK_STACK_DEPTH = 4096;

    constexpr const char* SYSTEM_TASK_NAME = "SystemTask";
    constexpr uint8_t SYSTEM_TASK_PRIORITY = 18;
    constexpr uint16_t SYSTEM_TASK_STACK_DEPTH = 8192;

    constexpr const char* PID_TASK_NAME = "PID_Task";
    constexpr uint8_t PID_TASK_PRIORITY = 15;
    constexpr uint16_t PID_TASK_STACK_DEPTH = 8192;

    constexpr const char* IO_TASK_NAME = "IO_Task";
    constexpr uint8_t IO_TASK_PRIORITY = 10;
    constexpr uint16_t IO_TASK_STACK_DEPTH = 4096;

    constexpr const char* LEDS_TASK_NAME = "LedsTask";
    constexpr uint8_t LEDS_TASK_PRIORITY = 3;
    constexpr uint16_t LEDS_TASK_STACK_DEPTH = 2048;

    constexpr const char* BUZZER_TASK_NAME = "BuzzerTask";
    constexpr uint8_t BUZZER_TASK_PRIORITY = 1;
    constexpr uint16_t BUZZER_TASK_STACK_DEPTH = 1024;

    constexpr const char* SENSOR_TEST_TASK_NAME = "SensorTestTask";
    constexpr uint8_t SENSOR_TEST_TASK_PRIORITY = 22;
    constexpr uint16_t SENSOR_TEST_TASK_STACK_DEPTH = 8192;
}