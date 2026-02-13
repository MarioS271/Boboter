/**
 * @file status_led_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr const char* TAG = "Task::status_led_task";
constexpr uint16_t FAST_BLINK_INTERVAL_MS = 500;
constexpr uint16_t SLOW_BLINK_INTERVAL_MS = 1000;

namespace Task {
    [[noreturn]] void status_led_task(void* params) {
        Robot& robot = Robot::get_instance();

        bool led_state = false;

        while (true) {
            using enum Robot::status_led_mode_t;
            const auto mode = robot.data->leds.status_led_mode;

            uint32_t interval = 0;
            switch (mode) {
                case BLINK_SLOW: interval = SLOW_BLINK_INTERVAL_MS; break;
                case BLINK_FAST: interval = FAST_BLINK_INTERVAL_MS; break;
                case OFF: interval = 0; robot.set_status_led(false); break;
                case ON: interval = 0; robot.set_status_led(true); break;
            }

            if (interval > 0) {
                robot.set_status_led(led_state);
                led_state = !led_state;
                delay(interval);
            } else {
                delay(100);
            }
        }
    }
}