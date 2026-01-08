/**
 * @file robot.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/robot.h"
#include "lib/logger/logger.h"

Robot::Robot() :
    gpio(HAL::GPIO::Controller::get_instance()),
    ledc(HAL::LEDC::Controller::get_instance()),
    adc(HAL::ADC::Controller::get_instance()),
    i2c(HAL::I2C::Controller::get_instance()),

    battery(*this),
    leds(*this),
    display(*this)
{
    LOGI("Constructor of Robot called");
}

Robot::~Robot() {
    using enum HAL::GPIO::level_t;

    LOGI("Destructor of Robot called");

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
        config.stack_depth,
        config.params_for_task,
        config.priority,
        config.created_task_handle,
        config.core_id
    );
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