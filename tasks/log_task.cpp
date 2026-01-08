/**
 * @file log_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "tasks.h"

#include "helpers/delay.h"
#include "lib/logger/logger.h"

namespace Task {
    [[noreturn]] void log_task(void* params) {
        while (true) {
            process_log_queue();
            delay(1);
        }
    }
}