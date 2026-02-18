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

        constexpr const char* TAG = "test";
        robot.encoders.reset_pulse_count(Device::Encoders::encoder_id_t::LEFT);
        robot.encoders.reset_pulse_count(Device::Encoders::encoder_id_t::RIGHT);

        while (true) {
            LOGI("Encoder count: LEFT=%ld RIGHT=%ld",
                robot.encoders.get_pulse_count(Device::Encoders::encoder_id_t::LEFT),
                robot.encoders.get_pulse_count(Device::Encoders::encoder_id_t::RIGHT));

            delay(500);
        }
    }
}