/**
 * @file system_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

static constexpr const char* TAG = "Task::system_task";

namespace Task {
    [[noreturn]] void system_task(void* params) {
        const Robot& robot = Robot::get_instance();

        using enum Robot::rgb_leds_mode_t;
        robot.data->leds.rgb_leds_mode = POLICE;

        while (true) {
            delay(1000);
        }
    }
}