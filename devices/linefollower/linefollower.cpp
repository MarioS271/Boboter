/**
 * @file linefollower.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "linefollower.h"

#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"

namespace Device {
    Linefollower::Linefollower(Robot& robot) :
        robot(robot),
        left_sensor_color(linefollower_result_t::BLACK),
        right_sensor_color(linefollower_result_t::BLACK)
    {
        LOGD("Constructor called");
    }

    Linefollower::~Linefollower() {
        LOGD("Destructor called");
    }

    void Linefollower::initialize() {
        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = LF_RIGHT_LEFT_PIN,
                .mode = GPIO_MODE_OUTPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        robot.adc.add(ADC_CHANNEL);

        LOGI("Initialized Device::Linefollower");
    }

    void Linefollower::measure() {
        using enum linefollower_id_t;
        using enum linefollower_result_t;

        for (uint8_t i = 0; i < 2; ++i) {
            robot.gpio.set_level(LF_RIGHT_LEFT_PIN, static_cast<HAL::GPIO::level_t>(i));
            delay(10);

            const uint16_t reading = robot.adc.read_raw(ADC_CHANNEL);

            linefollower_result_t result;
            if (reading > REFLECTIVITY_THRESHOLD) {
                result = BLACK;
            } else {
                result = WHITE;
            }

            LOGV("Measured %s (%hu) for %s sensor", result == BLACK ? "BLACK" : "WHITE", reading, i == 1 ? "LEFT" : "RIGHT");

            if (static_cast<linefollower_id_t>(i) == LEFT) {
                left_sensor_color = result;
            } else {
                right_sensor_color = result;
            }
        }
    }
}
