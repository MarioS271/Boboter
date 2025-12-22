/**
 * @file sensor_test.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "types/system_context.hpp"
#include "types/rgb_color.hpp"
#include "helpers/predef_colors.hpp"
#include "helpers/delay.hpp"

#include "lib/logger/logger.hpp"
#include "lib/rgb_leds/rgb_leds.hpp"
#include "lib/motor/motor.hpp"
#include "lib/encoder/encoder.hpp"
#include "lib/ultrasonic/ultrasonic.hpp"
#include "lib/bumper/bumper.hpp"

namespace Boboter::Tasks::SensorTest {
    namespace Config {
        constexpr float MAX_DISPLAY_DISTANCE = 100.0f;
        constexpr float MIN_DISPLAY_DISTANCE = 5.0f;
    }

    namespace Internal {
        constexpr const char* TAG = "Tasks::SensorTest";
    }

    void task(void* params);
}