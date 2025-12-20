/**
 * @file io_task.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <esp_timer.h>
#include "types/system_context.hpp"
#include "types/flex_struct.hpp"
#include "helpers/delay.hpp"
#include "lib/battery/battery.hpp"
#include "lib/io_shield/io_shield.hpp"

namespace Boboter::Task::IO
{
    namespace Constants
    {
        constexpr const char* TAG = "Task::IO";
    }

    void Task(void* params);
}