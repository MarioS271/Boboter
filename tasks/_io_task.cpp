/**
 * @file _io_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "tasks.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

constexpr uint8_t MAX_BATTERY_STRING_LENGTH = 8;

namespace Task {
    [[noreturn]] void io_task(void* params) {
        Robot& robot = Robot::get_instance();

        robot.encoders.reset_pulse_count(Device::Encoders::encoder_id_t::LEFT);
        robot.encoders.reset_pulse_count(Device::Encoders::encoder_id_t::RIGHT);

        robot.display.set_cursor_position(0, 0);
        robot.display.write_text("Hello, World! :)");

        while (true) {
            char percentage_str[MAX_BATTERY_STRING_LENGTH];
            snprintf(percentage_str, sizeof(percentage_str), "%d%%  ", robot.battery.get_percentage());

            robot.display.set_cursor_position(0, 1);
            robot.display.write_text("Battery: ");
            robot.display.write_text(percentage_str);

            char buffer[32];

            robot.ultrasonic.measure();
            snprintf(buffer, sizeof(buffer), "L%2ld R%2ld  D%2.1f",
                robot.encoders.get_pulse_count(Device::Encoders::encoder_id_t::LEFT),
                robot.encoders.get_pulse_count(Device::Encoders::encoder_id_t::RIGHT),
                robot.ultrasonic.get_distance());
            robot.display.set_cursor_position(0, 3);
            robot.display.write_text(buffer);

            multi_heap_info_t info;
            heap_caps_get_info(&info, MALLOC_CAP_8BIT);

            snprintf(buffer, sizeof(buffer), "Heap: %d KiB", info.total_allocated_bytes / 1024);
            robot.display.set_cursor_position(0, 6);
            robot.display.write_text(buffer);

            snprintf(buffer, sizeof(buffer), "Free: %d KiB", info.total_free_bytes / 1024);
            robot.display.set_cursor_position(0, 7);
            robot.display.write_text(buffer);

            delay(1000);
        }
    }
}