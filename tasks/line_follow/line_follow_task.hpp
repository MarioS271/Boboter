/**
 * @file line_follow_task.hpp
 *
 * @authors MarioS271, ThomS263
 * @copyright MIT License
*/

#pragma once

#include "types/system_context.hpp"
#include "types/flex_struct.hpp"
#include "helpers/delay.hpp"
#include "lib/logger/logger.hpp"
#include "lib/motors/motors.hpp"
#include "lib/linefollower/linefollower.hpp"

namespace Boboter::Task::LineFollow
{
    namespace Constants
    {
        constexpr const char* TAG = "Task::LineFollow";

        constexpr uint16_t BASE_SPEED = Motors::MAX_MOTOR_SPEED / 4;
        constexpr int16_t Kp = Motors::MAX_MOTOR_SPEED / 10;
        constexpr bool LOGGING_ENABLE = false;
    }

    void Task(void* params);
}