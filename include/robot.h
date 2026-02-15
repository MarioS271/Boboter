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
    static constexpr gpio_num_t STATUS_LED_PIN = GPIO_NUM_5;
    static constexpr gpio_num_t BOTTOM_LED_PIN = GPIO_NUM_13;

    struct task_config_t {
        TaskFunction_t task_function;
        const char* task_name;
        const uint32_t stack_size;
        void* const params_for_task;
        UBaseType_t priority;
        TaskHandle_t* const created_task_handle;
        const BaseType_t core_id;
    };

    enum class robot_mode_t : uint8_t {
        NONE = 0,
        APP = 1,
        GAMEPAD = 2
    };

    enum class status_led_mode_t : uint8_t {
        OFF = 0,
        ON = 1,
        BLINK_SLOW = 2,
        BLINK_FAST = 3,
    };

    enum class rgb_leds_mode_t : uint8_t {
        OFF = 0,
        HEADLIGHTS = 1,
        RANDOM_COLORS = 2,
        POLICE = 3
    };

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

    struct data_struct {
        robot_mode_t mode;

        struct leds_struct {
            status_led_mode_t status_led_mode;
            rgb_leds_mode_t rgb_leds_mode;
        } leds{};

        struct motors_struct {
            uint16_t target_speed_left;
            uint16_t target_speed_right;
            uint16_t ramp_time_left;
            uint16_t ramp_time_right;
            Device::Motors::motor_direction_t direction_left;
            Device::Motors::motor_direction_t direction_right;
        } motors{};
    };
    protected_struct<data_struct> data{};

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