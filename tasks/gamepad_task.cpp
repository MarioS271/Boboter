/**
 * @file gamepad_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr const char* TAG = "Task::gamepad_task";

namespace Task {
    [[noreturn]] void gamepad_task(void* params) {
        Robot& robot = Robot::get_instance();

        while (true) {
            delay(1000);
        }
    }
}