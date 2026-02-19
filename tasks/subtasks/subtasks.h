/**
 * @file subtasks.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <cstdint>

namespace Task::Subtask {
    using subtask_t = uint32_t(*)();

    constexpr uint32_t RERUN_IMMEDIATELY = 0;

    uint32_t display_subtask();
    uint32_t status_led_subtask();
    uint32_t rgb_leds_subtask();
    uint32_t buzzer_subtask();
}