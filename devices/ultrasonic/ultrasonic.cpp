/**
 * @file ultrasonic.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "ultrasonic.h"

#include <rom/ets_sys.h>
#include <esp_timer.h>
#include "include/robot.h"
#include "helpers/delay.h"

namespace Device {
    Ultrasonic::Ultrasonic(Robot& robot) :
        robot(robot),
        distance(-1)
    {
        LOGD("Constructor called");
    }

    Ultrasonic::~Ultrasonic() {
        LOGD("Destructor called");
    }

    void Ultrasonic::initialize() {
        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = TRIGGER_PIN,
                .mode = GPIO_MODE_OUTPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = ECHO_PIN,
                .mode = GPIO_MODE_INPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        LOGI("Initialized Device::Ultrasonic");
    }

    void Ultrasonic::measure() {
        using enum HAL::GPIO::level_t;

        robot.gpio.set_level(TRIGGER_PIN, LOW);
        ets_delay_us(1000);
        robot.gpio.set_level(TRIGGER_PIN, HIGH);
        ets_delay_us(10);
        robot.gpio.set_level(TRIGGER_PIN, LOW);

        const int64_t wait_start_time = esp_timer_get_time();
        while (robot.gpio.get_level(ECHO_PIN) == LOW) {
            if (esp_timer_get_time() - wait_start_time > ECHO_TIMEOUT_MS) {
                distance = -1;
                return;
            }
        }

        const int64_t echo_return_start_time = esp_timer_get_time();
        while (robot.gpio.get_level(ECHO_PIN) == HIGH) {
            if (esp_timer_get_time() - echo_return_start_time > ECHO_TIMEOUT_MS) {
                distance = -1;
                return;
            }
        }
        const int64_t echo_return_end_time = esp_timer_get_time();

        const auto echo_time = static_cast<float>(echo_return_end_time - echo_return_start_time);
        distance = echo_time / 58.0f;  // 343 m/s

        LOGV("Measured distance of %f cm (echo_time=%f)", distance, echo_time);
    }
}
