/**
 * @file test_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
*/

#include "tasks.h"

#include "helpers/delay.h"
#include "lib/logger/logger.h"
#include "include/robot.h"

namespace Task {
    [[noreturn]] void test_task(void* params) {
        Robot& robot = Robot::get_instance();

        while (true) {
            delay(1000);
        }
    }
}