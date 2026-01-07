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
    leds(*this)
{
    LOGI("Constructor of Robot called");
}

Robot::~Robot() {
    LOGI("Destructor of Robot called");

    gpio.shutdown();
    ledc.shutdown();
    adc.shutdown();
    i2c.shutdown();
}

void Robot::begin() {
    adc.configure(
        HAL::ADC::controller_config_t{
            .attenuation = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_12,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
            .clock_source = ADC_RTC_CLK_SRC_DEFAULT
        }
    );
    i2c.configure(
        HAL::I2C::bus_config_t{
            .port = I2C_NUM_0,
            .sda_pin = GPIO_NUM_21,
            .scl_pin = GPIO_NUM_22
        }
    );

    gpio.add(
        HAL::GPIO::pin_config_t{
            .gpio_pin = STATUS_LED_PIN,
            .mode = GPIO_MODE_OUTPUT,
            .pull_mode = GPIO_FLOATING,
            .intr_type = GPIO_INTR_DISABLE
        }
    );
    gpio.add(
        HAL::GPIO::pin_config_t{
            .gpio_pin = BOTTOM_LED_PIN,
            .mode = GPIO_MODE_OUTPUT,
            .pull_mode = GPIO_FLOATING,
            .intr_type = GPIO_INTR_DISABLE
        }
    );

    battery.initialize();
    leds.initialize();
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