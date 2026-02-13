/**
 * @file app_backend_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr const char* TAG = "Task::app_backend_task";

namespace Task {
    [[noreturn]] void app_backend_task(void* params) {
        Robot& robot = Robot::get_instance();

        while (true) {
            delay(1000);
        }
    }
}