/**
 * @file system_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
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
        robot.data->leds.rgb_leds_mode = RANDOM_COLORS;

        while (true) {
            robot.motors.set_direction(Device::Motors::motor_id_t::LEFT, Device::Motors::motor_direction_t::FORWARD);
            robot.motors.set_speed(Device::Motors::motor_id_t::LEFT, 500);
            delay(5000);
            robot.motors.stop(Device::Motors::motor_id_t::LEFT);
            delay(3000);

            robot.motors.set_direction(Device::Motors::motor_id_t::LEFT, Device::Motors::motor_direction_t::BACKWARD);
            robot.motors.set_speed(Device::Motors::motor_id_t::LEFT, 500);
            delay(5000);
            robot.motors.stop(Device::Motors::motor_id_t::LEFT);
            delay(3000);
        }
    }
}