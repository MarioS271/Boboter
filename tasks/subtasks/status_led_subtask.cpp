/**
 * @file status_led_subtask.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "subtasks.h"

#include <esp_timer.h>
#include "include/robot.h"

constexpr uint8_t POLL_DELAY_MS = 50;
constexpr uint32_t SLOW_BLINK_INTERVAL_US = 500'000;
constexpr uint32_t FAST_BLINK_INTERVAL_US = 250'000;

namespace Task::Subtask {
    void status_led_subtask_init() {}

    uint32_t status_led_subtask() {
        using enum Robot::status_led_mode_t;

        static Robot& robot = Robot::get_instance();
        static Robot::status_led_mode_t mode{};
        static bool led_state = false;
        static int64_t last_blink_time = 0;
        static uint32_t interval = 0;

        if (const Robot::status_led_mode_t new_mode = robot.data->leds.status_led_mode;
            new_mode != mode)
        {
            mode = new_mode;
            led_state = false;

            switch (mode) {
                case OFF: robot.set_status_led(false); break;
                case ON: robot.set_status_led(true); break;
                case BLINK_SLOW: interval = SLOW_BLINK_INTERVAL_US; break;
                case BLINK_FAST: interval = FAST_BLINK_INTERVAL_US; break;
            }
        }

        if (interval > 0) {
            const int64_t current_time = esp_timer_get_time();
            if (current_time - last_blink_time >= interval) {
                last_blink_time += interval;
                led_state = !led_state;
                robot.set_status_led(led_state);
            }
        }

        return POLL_DELAY_MS;
    }
}