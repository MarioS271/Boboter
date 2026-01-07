/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/constants.h"
#include "include/flags.h"
#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

extern "C" void app_main() {
    using namespace Constants;

    LOGI("=== Boboter is starting ===");
    LOGI("Hello, World!");

    Robot& robot = Robot::get_instance();
    robot.begin();

    while (true) {
        robot.battery.update();
        const int8_t percentage = robot.battery.get_percentage();
        const uint16_t voltage = robot.battery.get_millivolts();

        LOGI("Battery Percentage/Voltage: %d%% / %d mV", percentage, voltage);

        delay(500);
    }
}
