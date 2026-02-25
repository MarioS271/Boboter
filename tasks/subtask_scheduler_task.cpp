/**
 * @file subtask_scheduler_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks.h"

#include <array>
#include <esp_timer.h>
#include "helpers/delay.h"
#include "include/flags.h"
#include "include/robot.h"
#include "tasks/subtasks/subtasks.h"

namespace Subtask = Task::Subtask;

constexpr const char* TAG = "Task::subtask_scheduler_task";
constexpr uint8_t SUBTASK_LOOP_YIELD_MS = 5;

template <typename T>
constexpr std::array<T, Subtask::subtask_count()> create_callback_array() {
    std::array<T, Subtask::subtask_count()> callbacks;
    size_t i = 0;

    if constexpr (std::is_same_v<T, Subtask::subtask_init_callback_t>) {
        callbacks[i++] = Subtask::status_led_subtask_init;

        if constexpr (!Flags::ENABLE_TEST_MODE) {
            callbacks[i++] = Subtask::rgb_leds_subtask_init;

            if constexpr (Flags::ENABLE_DISPLAY) {
                callbacks[i++] = Subtask::display_subtask_init;
            }
            if constexpr (Flags::ENABLE_BUZZER) {
                callbacks[i++] = Subtask::buzzer_subtask_init;
            }
        }
    } else {
        callbacks[i++] = Subtask::status_led_subtask;

        if constexpr (!Flags::ENABLE_TEST_MODE) {
            callbacks[i++] = Subtask::rgb_leds_subtask;

            if constexpr (Flags::ENABLE_DISPLAY) {
                callbacks[i++] = Subtask::display_subtask;
            }
            if constexpr (Flags::ENABLE_BUZZER) {
                callbacks[i++] = Subtask::buzzer_subtask;
            }
        }
    }

    return callbacks;
}

constexpr std::array<Subtask::subtask_init_callback_t, Subtask::subtask_count()> subtask_init_callbacks = create_callback_array<Subtask::subtask_init_callback_t>();
constexpr std::array<Subtask::subtask_callback_t, Subtask::subtask_count()> subtask_callbacks = create_callback_array<Subtask::subtask_callback_t>();

namespace Task {
    [[noreturn]] void subtask_scheduler_task(void* params) {
        using namespace Task::Subtask;
        
        int64_t next_run_timestamps[subtask_count()]{};

        for (const auto& callback : subtask_init_callbacks) {
            callback();
        }

        const int64_t _current_time = esp_timer_get_time();
        for (auto& timestamp : next_run_timestamps) {
            timestamp = _current_time;
        }

        while (true) {
            for (uint8_t i = 0; i < subtask_count(); ++i) {
                const int64_t current_time = esp_timer_get_time();

                if (current_time >= next_run_timestamps[i]) {
                    const uint32_t requested_delay = subtask_callbacks[i]();
                    next_run_timestamps[i] = current_time + (requested_delay * 1000);
                }
            }

            delay(SUBTASK_LOOP_YIELD_MS);
        }
    }
}