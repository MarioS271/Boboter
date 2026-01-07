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
#include "devices/battery/battery.h"

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

    HAL::GPIO::Controller& gpio;
    HAL::LEDC::Controller& ledc;
    HAL::ADC::Controller& adc;
    HAL::I2C::Controller& i2c;

    Device::Battery battery;

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
     * @brief Initialize all HALs, devices and integrated stuff like the status LED or similar
     */
    void begin();

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