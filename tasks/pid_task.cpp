/**
 * @file pid_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

namespace Task {
    [[noreturn]] void pid_task(void* params) {
        Robot& robot = Robot::get_instance();

        while (true) {
            constexpr const char* TAG = "test";

            robot.linefollower.measure();
            LOGD("Left Sensor: %s", robot.linefollower.get_left_sensor_color() == Device::Linefollower::linefollower_result_t::WHITE ? "WHITE" : "BLACK");
            LOGD("Right Sensor: %s", robot.linefollower.get_right_sensor_color() == Device::Linefollower::linefollower_result_t::WHITE ? "WHITE" : "BLACK");
            delay(500);

            // delay(1000);
        }
    }
}