/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include <esp_random.h>
#include "include/constants.h"
#include "include/flags.h"
#include "include/robot.h"
#include "helpers/delay.h"
#include "helpers/predef_colors.h"
#include "lib/logger/logger.h"

extern "C" void app_main() {
    using namespace Constants;

    LOGI("=== Boboter is starting ===");
    LOGI("Hello, World!");

    Robot& robot = Robot::get_instance();
    robot.begin();

    while (true) {
        for (int i = 0; i < 4; ++i) {
            const uint32_t color_index = esp_random() % Colors::NUM_COLORS;

            robot.leds.set_color(
                static_cast<Device::Leds::led_id_t>(i),
                Colors::LIST[color_index]
            );
        }

        delay(1000);
    }
}