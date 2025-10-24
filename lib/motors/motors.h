// MOTORS_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"

constexpr uint16_t MAX_MOTOR_SPEED = 1023;

enum motor_num_t : uint8_t {
    MOTOR_LEFT = 0,
    MOTOR_RIGHT = 1
};
enum motor_direction_t : uint8_t {
    FORWARD = 0,
    BACKWARD = 1
};

class Motor {
private:
    // Pin Configuration
    static constexpr gpio_num_t MOTOR_LEFT_SPEED_PIN = GPIO_NUM_32;
    static constexpr gpio_num_t MOTOR_LEFT_DIRECTION_PIN = GPIO_NUM_33;
    static constexpr gpio_num_t MOTOR_RIGHT_SPEED_PIN = GPIO_NUM_2;
    static constexpr gpio_num_t MOTOR_RIGHT_DIRECTION_PIN = GPIO_NUM_15;

    // LEDC Configuration
    static constexpr ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;
    static constexpr ledc_timer_bit_t LEDC_RESOLUTION = LEDC_TIMER_10_BIT;
    static constexpr uint32_t LEDC_FREQUENCY = 10000;   // 10 kHz

    // Ramp Configuration
    static constexpr uint8_t RAMP_STEP = 20;
    static constexpr uint8_t RAMP_INTERVAL_MS = 10;

    // Ramp Variables
    uint16_t target_speed;
    TaskHandle_t rampTaskHandle;

    // Motor Hardware Info
    motor_num_t motor_num;
    gpio_num_t speed_pin;
    gpio_num_t direction_pin;
    ledc_channel_t ledc_channel;
    bool inverse_direction;
    bool error;

    // Current State Variables
    uint16_t current_speed;
    motor_direction_t current_direction;

    // Private Functions
    static void rampTask(void* pvParameters);

public:
    explicit Motor(motor_num_t motor_number);

    void stop(bool wait = true);
    void setSpeed(uint16_t speed);
    void setDirection(motor_direction_t direction);

    uint16_t getSpeed();
    motor_direction_t getDirection();
    motor_direction_t getActualDirection(motor_direction_t);

    bool hasError();
};