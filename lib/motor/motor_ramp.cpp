/**
 * @file motor_ramp.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "motor.hpp"

#include <algorithm>
#include "helpers/delay.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Motor {
    void Internal::ramp_task(void* params) {
        using namespace Config;
        using Boboter::Helpers::delay;
        using namespace Boboter::Libs::Error;

        Motor* motor = static_cast<Motor*>(params);

        while (true) {
            if (motor->current_speed != motor->target_speed) {
                int16_t current = static_cast<int16_t>(motor->current_speed);
                int16_t target = static_cast<int16_t>(motor->target_speed);

                if (current < target)
                    current = std::min<int16_t>(current + RAMP_STEP, target);
                else if (current > target)
                    current = std::max<int16_t>(current - RAMP_STEP, target);

                current = std::clamp<int16_t>(current, 0, 1023);
                motor->current_speed = current;

                WARN_CHECK(Motor::TAG, ledc_set_duty(LEDC_SPEED_MODE, motor->ledc_channel, static_cast<uint16_t>(current)));
                WARN_CHECK(Motor::TAG, ledc_update_duty(LEDC_SPEED_MODE, motor->ledc_channel));
            }

            delay(RAMP_INTERVAL_MS);
        }
    }
}