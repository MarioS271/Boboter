/**
 * @file flags.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

/**
 * @brief A namespace containing flags for @c app_main
 */
namespace Flags {
    // ENABLE_SENSOR_TEST_MODE
    //  This flag decides if the Robot will operate in "Sensor Test Mode",
    //  which tests the robot's sensors and functions
    constexpr bool ENABLE_SENSOR_TEST_MODE = false;
}