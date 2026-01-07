/**
 * @file robot.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include "include/hal/gpio.h"
#include "include/hal/ledc.h"
#include "include/hal/adc.h"
#include "include/hal/i2c.h"

/**
 * @brief Class containing all the components of the robot
 */
class Robot {
private:
    static constexpr const char* TAG = "Robot";

private:
    explicit Robot();
    ~Robot();

public:
    static constexpr gpio_num_t STATUS_LED_PIN = GPIO_NUM_5;
    static constexpr gpio_num_t BOTTOM_LED_PIN = GPIO_NUM_13;

public:
    Robot(const Robot&) = delete;
    Robot& operator=(const Robot&) = delete;

    GPIO::Controller& gpio;
    LEDC::Controller& ledc;
    ADC::Controller& adc;
    I2C::Controller& i2c;

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
};