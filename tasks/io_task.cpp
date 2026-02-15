/**
 * @file io_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks/tasks.h"

#include "include/robot/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr uint8_t MAX_BATTERY_STRING_LENGTH = 8;

namespace Task {
    [[noreturn]] void io_task(void* params) {
        Robot& robot = Robot::get_instance();

        // robot.display.write_text("Hello, World!");

        while (true) {
            char percentage_str[MAX_BATTERY_STRING_LENGTH];
            snprintf(percentage_str, sizeof(percentage_str), "%d%%  ", robot.battery.get_percentage());

            robot.display.set_cursor_position(0, 0);
            robot.display.write_text(percentage_str);

            multi_heap_info_t info;
            heap_caps_get_info(&info, MALLOC_CAP_8BIT);

            char buffer[32];

            // Used Heap RAM
            snprintf(buffer, sizeof(buffer), "Heap: %d KB", info.total_allocated_bytes / 1024);
            robot.display.set_cursor_position(0, 30);
            robot.display.write_text(buffer);

            // Free Heap Ram
            snprintf(buffer, sizeof(buffer), "Free: %d KB", info.total_free_bytes / 1024);
            robot.display.set_cursor_position(0, 40);
            robot.display.write_text(buffer);

            delay(1000);
        }
    }
}