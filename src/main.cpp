/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/flags.h"
#include "include/constants.h"
#include "include/robot.h"
#include "lib/logger/logger.h"

#include "helpers/delay.h"
#include <esp_random.h>
#include "helpers/predef_colors.h"

extern "C" void app_main() {
    constexpr const char* TAG = "Main";

    LOGI("=== Boboter is starting ===");
    LOGI("Firmware v%s by %s", Constants::VERSION, Constants::AUTHORS);
    LOGI("Hello, World!");
    LOGI("===========================");

    LOGW("Warning");
    LOGE("Error");
    LOGD("Debug");
    LOGV("Verbose");

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