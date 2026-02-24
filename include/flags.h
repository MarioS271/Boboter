/**
 * @file flags.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <esp_log_level.h>
#include "include/log_sources.h"

/**
 * @brief A namespace containing flags for configuring the firmware
 */
namespace Flags {
    /**
     * @brief Enable the robot's test mode
     * @note Default Value: @c false
     *
     * @details This disables most of the robot's tasks and enables a special
     *          test task, which goes through most of the robot's devices and tests
     *          them one by one.
     */
    constexpr bool ENABLE_TEST_MODE = true;



    ///// LOGGING OPTIONS /////
    /**
     * @brief Enable logging in color over the serial console
     * @note Default Value: @c true
     */
    constexpr bool ENABLE_COLOR_LOGGING = true;

    /**
     * @brief Enable logging of debug messages
     * @note Default Value: @c ESP_LOG_INFO
     */
    constexpr esp_log_level_t LOWEST_LOG_LEVEL = ESP_LOG_VERBOSE;

    /**
     * @brief Enable specific modules to output verbose logs
     * @note Default Value: @c LOG_SOURCE_NONE
     */
    constexpr uint32_t ENABLED_VERBOSE_LOG_SOURCES = LOG_SOURCE_DEVICE_IMU | LOG_SOURCE_DEVICE_ULTRASONIC;



    ///// HARDWARE OPTIONS /////
    /**
     * @brief Enables the SSD1306 OLED display
     * @note Default Value: @c false
     */
    constexpr bool ENABLE_DISPLAY = true;

    /**
     * @brief Enables the buzzer
     * @note Default Value: @c false
     */
    constexpr bool ENABLE_BUZZER = true;

    /**
     * @brief Enables the MPU6050 IMU and DMP
     * @note Default Value: @c false
     */
    constexpr bool ENABLE_IMU = true;
}