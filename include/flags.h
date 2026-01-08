/**
 * @file flags.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

/**
 * @brief A namespace containing flags for configuring the firmware
 */
namespace Flags {
    /**
     * @brief Enable the robot's test mode
     * @note Default Value: false
     *
     * @details This disables most of the robot's tasks and enables a special
     *          test task, which goes through most of the robot's devices and tests
     *          them one by one.
     */
    constexpr bool ENABLE_TEST_MODE = false;

    /**
     * @brief Enable logging in color over the serial console
     * @note Default Value: true
     */
    constexpr bool ENABLE_COLOR_LOGGING = true;
}