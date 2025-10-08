// MOTORS_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/gpio.h"
#include "driver/ledc.h"

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
    static constexpr gpio_num_t MOTOR_LEFT_SPEED_PIN = GPIO_NUM_32;
    static constexpr gpio_num_t MOTOR_LEFT_DIRECTION_PIN = GPIO_NUM_33;
    static constexpr gpio_num_t MOTOR_RIGHT_SPEED_PIN = GPIO_NUM_2;
    static constexpr gpio_num_t MOTOR_RIGHT_DIRECTION_PIN = GPIO_NUM_15;

    static constexpr ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;
    static constexpr ledc_timer_bit_t LEDC_RESOLUTION = LEDC_TIMER_10_BIT;
    static constexpr uint32_t LEDC_FREQUENCY = 10000;   // 10 kHz

    motor_num_t motor_num;
    gpio_num_t speed_pin;
    gpio_num_t direction_pin;
    ledc_channel_t ledc_channel;
    bool error;

    uint16_t saved_speed;
    motor_direction_t saved_direction;

public:
    explicit Motor(motor_num_t motor_number);

    void brake();
    void coast();
    void setSpeed(uint16_t speed);
    void setDirection(motor_direction_t direction);

    uint16_t getSpeed();
    motor_direction_t getDirection();
};