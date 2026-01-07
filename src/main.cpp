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

extern "C" void app_main() {
    using namespace Constants;

    const Robot& robot = Robot::get_instance();

    robot.ledc.add_timer(
        LEDC::timer_config_t{
            .timer = LEDC_TIMER_0,
            .frequency = 10'000,
            .resolution = LEDC_TIMER_10_BIT,
        }
    );

    robot.ledc.add_channel(
        LEDC::channel_config_t{
            .channel = LEDC_CHANNEL_0,
            .timer = LEDC_TIMER_0,
            .gpio_pin = GPIO_NUM_2,
            .duty = 0
        }
    );

    uint32_t duty = 0;

    while (true) {
        duty += 200;

        if (duty >= 1023) {
            duty = 0;
        }

        robot.ledc.set_duty(LEDC_CHANNEL_0, duty);

        delay(500);
    }
}