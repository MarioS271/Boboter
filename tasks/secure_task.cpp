/**
 * @file secure_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr const char* TAG = "Task::secure_task";
constexpr uint16_t MIN_BATTERY_VOLTAGE = 3'300;
constexpr uint16_t ALERT_DELAY = 1000;
constexpr uint16_t SHUTDOWN_DELAY = 5000;

enum class secure_task_state_t : uint8_t {
    OK = 0,
    WAITING = 1,
    ALERTING = 2,
    SHUTDOWN = 3
};

/**
 * @brief Helper function to convert @c secure_task_state_t enum class to string for logging
 */
const char* secure_task_state_enum_to_string(const secure_task_state_t state) {
    switch (state) {
        case secure_task_state_t::OK: return "OK";
        case secure_task_state_t::WAITING: return "WAITING";
        case secure_task_state_t::ALERTING: return "ALERTING";
        case secure_task_state_t::SHUTDOWN: return "SHUTDOWN";
        default: return nullptr;
    }
}

namespace Task {
    [[noreturn]] void secure_task(void* params) {
        using enum secure_task_state_t;

        Robot& robot = Robot::get_instance();

        secure_task_state_t state = OK;
        uint16_t voltage = 0;
        uint64_t current_time = 0;
        uint64_t too_low_start_time = 0;

        while (true) {
            robot.battery.update();
            voltage = robot.battery.get_millivolts();
            current_time = esp_timer_get_time();

            if (voltage < MIN_BATTERY_VOLTAGE) {
                if (state == OK) {
                    state = WAITING;
                    too_low_start_time = current_time;

                    LOGI("Detected low battery (%d mV), verifying reading...", voltage);
                }
            } else if (state != OK) {
                state = OK;
                robot.data->leds.status_led_mode = Robot::status_led_mode_t::ON;
            }

            if (state == WAITING && current_time - too_low_start_time >= ALERT_DELAY * 1000) {
                state = ALERTING;

                LOGW("Low battery warning (%d mV), robot will shut down in a few seconds!", voltage);

                robot.data->leds.status_led_mode = Robot::status_led_mode_t::BLINK_FAST;
                // Send sound to play:
                // { {E5, 100}, {E4, 100}, {C5, 100}, {C4, 100} }
            }

            if (state == ALERTING && current_time - too_low_start_time >= (SHUTDOWN_DELAY + ALERT_DELAY) * 1000) {
                robot.permanent_sleep();
            }

            delay(500);
        }
    }
}