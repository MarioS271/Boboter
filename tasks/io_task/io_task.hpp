/**
 * @file io_task.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

namespace Boboter::Tasks::IO {
    namespace Constants {
        constexpr const char* TAG = "Task::IO";
    }

    void task(void* params);
}