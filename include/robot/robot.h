/**
 * @file robot.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include "types/protected_struct.h"
#include "include/log_sources.h"
#include "include/hal/gpio.h"
#include "include/hal/ledc.h"
#include "include/hal/adc.h"
#include "include/hal/i2c.h"
#include "devices/battery/battery.h"
#include "devices/leds/leds.h"
#include "devices/display/display.h"
#include "devices/buttons/buttons.h"
#include "devices/buzzer/buzzer.h"
#include "devices/motors/motors.h"
#include "devices/bumpers/bumpers.h"
#include "devices/ultrasonic/ultrasonic.h"
#include "devices/linefollower/linefollower.h"

/**
 * @brief Class containing all the components of the robot
 */
class Robot {
private:
    static constexpr const char* TAG = "Robot";
    static constexpr log_source LOG_SOURCE = LOG_SOURCE_CLASS_ROBOT;

    explicit Robot();
    ~Robot();

public:
    #include "public_defs.inc"

public:
    HAL::GPIO::Controller& gpio;
    HAL::LEDC::Controller& ledc;
    HAL::ADC::Controller& adc;
    HAL::I2C::Controller& i2c;

    Device::Battery battery;
    Device::Leds leds;
    Device::Display display;
    Device::Buttons buttons;
    Device::Buzzer buzzer;
    Device::Motors motors;
    Device::Bumpers bumpers;
    Device::Ultrasonic ultrasonic;
    Device::Linefollower linefollower;

    #include "data_struct.inc"

public:
    Robot(const Robot&) = delete;
    Robot& operator=(const Robot&) = delete;

    /**
     * @brief Gets the robot instance
     * @note The instance will be created on the first call of this function
     *
     * @return Reference to the robot object
     */
    static Robot& get_instance() {
        static Robot _instance;
        return _instance;
    }

    /**
     * @brief Creates a FreeRTOS task
     *
     * @param config The configuration of the task to create
     */
    void create_task(const task_config_t& config);

    /**
     * @brief Shuts down all HALs, pulls down a few pins and puts the ESP32 into deep sleep mode
     */
    void permanent_sleep();

    /**
     * @brief Sets the status LED
     *
     * @param state Whether the LED should be turned on (true) or off (false)
     */
    void set_status_led(bool state) const;

    /**
     * @brief Sets the bottom LED state
     *
     * @param state Whether the LED should be turned on (true) or off (false)
     */
    void set_bottom_led(bool state) const;
};