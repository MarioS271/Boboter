/**
* @file buzzer_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
*/

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

namespace Task {
    [[noreturn]] void buzzer_task(void* params) {
        Robot& robot = Robot::get_instance();

        while (true) {
            delay(1000);
        }
    }
}