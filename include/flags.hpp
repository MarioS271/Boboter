/**
 * @file flags.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

namespace Boboter::Main::Flags {
    // ENABLE_SENSOR_TEST_MODE
    //  This flag decides if the Robot will operate in "Sensor Test Mode",
    //  which tests the robot's sensors and functions
    constexpr bool ENABLE_SENSOR_TEST_MODE = false;
    

    
    // ENABLE_DISPLAY
    //  Use to enable or disable the display management task
    constexpr bool ENABLE_DISPLAY = true;

    // ENABLE_BUTTONS
    //  Use to enable or disable the buttons on GPIO0 and GPIO5
    constexpr bool ENABLE_BUTTONS = true;

    // ENABLE_GYRO
    //  Use to enable or disable the gyro
    constexpr bool ENABLE_GYRO = true;

    // ENABLE_MAGNETOMETER
    //  Use to enable or disable the magnetometer
    constexpr bool ENABLE_MAGNETOMETER = true;
}