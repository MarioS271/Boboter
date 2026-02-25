/**
 * @file subtasks.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <cstdint>
#include "include/flags.h"

namespace Task::Subtask {
    using subtask_callback_t = uint32_t(*)();
    using subtask_init_callback_t = void(*)();

    constexpr uint8_t subtask_count() {
        uint8_t num_tasks = 1;

        if constexpr (!Flags::ENABLE_TEST_MODE) {
            ++num_tasks;

            if constexpr (Flags::ENABLE_DISPLAY) {
                ++num_tasks;
            }
            if constexpr (Flags::ENABLE_BUZZER) {
                ++num_tasks;
            }
        }

        return num_tasks;
    }

    void status_led_subtask_init();
    void rgb_leds_subtask_init();
    void display_subtask_init();
    void buzzer_subtask_init();

    uint32_t status_led_subtask();
    uint32_t rgb_leds_subtask();
    uint32_t display_subtask();
    uint32_t buzzer_subtask();
}