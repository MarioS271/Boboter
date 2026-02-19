/**
 * @file status_led_subtask.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "subtasks.h"

namespace Task::Subtask {
    uint32_t status_led_subtask() {
        return RERUN_IMMEDIATELY;
    }
}