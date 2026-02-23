/**
 * @file sensor_reader_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks.h"

#include "helpers/delay.h"
#include "lib/logger/logger.h"
#include "include/robot.h"

constexpr const char* TAG = "Task::sensor_reader_task";

namespace Task {
    [[noreturn]] void sensor_reader_task(void* params) {
        Robot& robot = Robot::get_instance();

        while (true) {
            robot.colorsensor.measure();
            robot.leds.set_color_all(robot.colorsensor.get_color());
            delay(100);
        }
    }
}