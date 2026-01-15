/**
 * @file io_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "tasks/tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr uint8_t MAX_BATTERY_STRING_LENGTH = 8;

namespace Task {
    [[noreturn]] void io_task(void* params) {
        Robot& robot = Robot::get_instance();

        robot.display.write_text("Hello, World!");

        while (true) {
            char percentage_str[MAX_BATTERY_STRING_LENGTH];
            snprintf(percentage_str, sizeof(percentage_str), "%d%%  ", robot.battery.get_percentage());

            robot.display.set_cursor_position(0, 10);
            robot.display.write_text(percentage_str);

            delay(1000);
        }
    }
}