/**
 * @file ramp_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"
#include "devices/motors/motors.h"

constexpr const char* TAG = "Task::ramp_task";
constexpr uint16_t RAMP_DELAY_MS = 50;

namespace Task {
    [[noreturn]] void ramp_task(void* params) {
        Robot& robot = Robot::get_instance();

        uint16_t target_speed[Device::Motors::NUM_MOTORS] = {0};
        uint16_t current_speed[Device::Motors::NUM_MOTORS] = {0};
        int16_t steps_per_cycle[Device::Motors::NUM_MOTORS] = {0};

        while (true) {
            auto locked_data = robot.data.lock();

            const uint16_t _set_target_speed[Device::Motors::NUM_MOTORS] = {
                locked_data->motors.target_speed_left,
                locked_data->motors.target_speed_right
            };
            const uint16_t _set_ramp_time[Device::Motors::NUM_MOTORS] = {
                locked_data->motors.ramp_time_left,
                locked_data->motors.ramp_time_right
            };
            const Device::Motors::motor_direction_t _set_direction[Device::Motors::NUM_MOTORS] = {
                locked_data->motors.direction_left,
                locked_data->motors.direction_right
            };

            for (uint8_t i = 0; i < Device::Motors::NUM_MOTORS; ++i) {
                const auto motor_id = static_cast<Device::Motors::motor_id_t>(i);

                if (target_speed[i] != _set_target_speed[i]) {
                    target_speed[i] = _set_target_speed[i];
                    robot.motors._set_direction(motor_id, _set_direction[i]);

                    uint16_t cycles = _set_ramp_time[i] / RAMP_DELAY_MS;
                    if (cycles == 0) cycles = 1;
                    steps_per_cycle[i] = (static_cast<int32_t>(target_speed[i]) - current_speed[i]) / cycles;
                }

                if (current_speed[i] != target_speed[i]) {
                    const int32_t next_speed = current_speed[i] + steps_per_cycle[i];

                    if ((steps_per_cycle[i] > 0 && next_speed > target_speed[i]) ||
                        (steps_per_cycle[i] < 0 && next_speed < target_speed[i]))
                    {
                        current_speed[i] = target_speed[i];
                    } else {
                        current_speed[i] = static_cast<uint16_t>(next_speed);
                    }

                    robot.motors._set_speed(motor_id, current_speed[i]);
                }
            }

            locked_data.unlock();
            delay(RAMP_DELAY_MS);
        }
    }
}