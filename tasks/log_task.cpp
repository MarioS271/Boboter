/**
 * @file log_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks.h"

#include "lib/logger/logger.h"

namespace Task {
    [[noreturn]] void log_task(void* params) {
        while (true) {
            Logger::get_instance().process_log_queue();
        }
    }
}