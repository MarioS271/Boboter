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
        delay(2);
        robot.gpio.set_level(TRIGGER_PIN, HIGH);
        ets_delay_us(10);
        robot.gpio.set_level(TRIGGER_PIN, LOW);

        const int64_t wait_for_echo_time = esp_timer_get_time();

        while (robot.gpio.get_level(ECHO_PIN) == LOW) {
            if (esp_timer_get_time() - wait_for_echo_time > ECHO_TIMEOUT_US) {
                LOGW("Timeout while waiting for echo pin to go high");
                return;
            }
            taskYIELD();
        }

        const int64_t echo_start_time = esp_timer_get_time();

        while (robot.gpio.get_level(ECHO_PIN) == HIGH) {
            if (esp_timer_get_time() - echo_start_time > ECHO_TIMEOUT_US) {
                LOGW("Timeout while waiting for the echo pin to go low");
                return;
            }
            taskYIELD();
        }

        const int64_t echo_end_time = esp_timer_get_time();

        const auto echo_time = static_cast<float>(echo_end_time - echo_start_time);
        const float measured_distance = echo_time / 58.0f;  // 343 m/s

        if (measured_distance > MAX_DISTANCE) {
            this->distance = 999.0f;
        } else {
            this->distance = measured_distance;
        }

        LOGV("Measured distance of %f cm (echo_time=%f)", this->distance, echo_time);
    }
}
