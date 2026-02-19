/**
 * @file subtask_scheduler_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks.h"

#include <esp_timer.h>
#include "helpers/delay.h"
#include "lib/logger/logger.h"
#include "include/robot.h"
#include "tasks/subtasks/subtasks.h"

namespace Subtask = Task::Subtask;

constexpr const char* TAG = "Task::subtask_scheduler_task";
constexpr uint8_t SUBTASK_LOOP_YIELD_MS = 1;
constexpr Subtask::subtask_t subtask_functions[] = {
    Subtask::display_subtask,
    Subtask::status_led_subtask,
    Subtask::rgb_leds_subtask,
    Subtask::buzzer_subtask
};
constexpr uint8_t NUM_SUBTASKS = std::size(subtask_functions);

namespace Task {
    [[noreturn]] void subtask_scheduler_task(void* params) {
        int64_t next_run_timestamps[NUM_SUBTASKS]{};

        const int64_t _current_time = esp_timer_get_time();
        for (uint8_t i = 0; i < NUM_SUBTASKS; ++i) {
            next_run_timestamps[i] = _current_time;
        }

        while (true) {
            for (uint8_t i = 0; i < NUM_SUBTASKS; ++i) {
                const int64_t current_time = esp_timer_get_time();

                if (current_time >= next_run_timestamps[i]) {
                    const uint32_t requested_delay = subtask_functions[i]();
                    next_run_timestamps[i] = current_time + (requested_delay * 1000);
                }
            }

            delay(SUBTASK_LOOP_YIELD_MS);
        }
    }
}