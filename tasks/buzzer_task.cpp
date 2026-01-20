/**
* @file buzzer_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

namespace Task {
    [[noreturn]] void buzzer_task(void* params) {
        Robot& robot = Robot::get_instance();

        constexpr uint16_t LOW_TONE = 435;
        constexpr uint16_t HIGH_TONE = 580;
        constexpr uint32_t INTERVAL = 500;

        while (true) {
            for (int i = 0; i < 5; i++) {
                robot.buzzer.set_frequency(HIGH_TONE);
                delay(INTERVAL);

                robot.buzzer.set_frequency(LOW_TONE);
                delay(INTERVAL);
            }
        }
    }
}