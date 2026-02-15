/**
 * @file robot.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include <esp_sleep.h>
#include "include/robot.h"
#include "helpers/delay.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

Robot::Robot() :
    gpio(HAL::GPIO::Controller::get_instance()),
    ledc(HAL::LEDC::Controller::get_instance()),
    adc(HAL::ADC::Controller::get_instance()),
    i2c(HAL::I2C::Controller::get_instance()),

    battery(*this),
    leds(*this),
    display(*this),
    buttons(*this),
    buzzer(*this),
    motors(*this),
    bumpers(*this),
    encoders(*this),
    ultrasonic(*this),
    linefollower(*this)
{
    LOGD("Constructor called");
}

Robot::~Robot() {
    using enum HAL::GPIO::level_t;

    LOGD("Destructor called");

    gpio.set_level(STATUS_LED_PIN, HIGH);
    gpio.set_level(BOTTOM_LED_PIN, LOW);

    gpio.shutdown();
    ledc.shutdown();
    adc.shutdown();
    i2c.shutdown();
}

void Robot::create_task(const task_config_t& config) {
    xTaskCreatePinnedToCore(
        config.task_function,
        config.task_name,
        config.stack_size,
        config.params_for_task,
        config.priority,
        config.created_task_handle,
        config.core_id
    );
    LOGI("Created task \"%s\" with priority %u and %lu KiB of stack memory",
        config.task_name,
        config.priority,
        config.stack_size / 1024);
}

void Robot::permanent_sleep() {
    LOGW("permanent_sleep() called");
    LOGW("  -> Shutting down HALs");

    leds.shutdown();
    display.shutdown();

    gpio.shutdown();
    ledc.shutdown();
    adc.shutdown();
    i2c.shutdown();

    LOGW("  -> Preparing for deep sleep");

    WARN_CHECK(esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL));
    WARN_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON));

    LOGW("Cutting CPU power and entering deep sleep now. Bye!");

    delay(100);
    esp_deep_sleep_start();

    LOGE("Failed to enter deep sleep, aborting");
    abort();
}

void Robot::set_status_led(const bool state) const {
    gpio.set_level(
        STATUS_LED_PIN,
        static_cast<HAL::GPIO::level_t>(!state)
    );
}

void Robot::set_bottom_led(const bool state) const {
    gpio.set_level(
        BOTTOM_LED_PIN,
        static_cast<HAL::GPIO::level_t>(state)
    );
}