/**
 * @file motor.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "motor_types.hpp"

#include <driver/ledc.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

namespace Boboter::Libs::Motor {
    namespace Config {
        constexpr gpio_num_t MOTOR_LEFT_SPEED_PIN = GPIO_NUM_32;
        constexpr gpio_num_t MOTOR_LEFT_DIRECTION_PIN = GPIO_NUM_33;
        constexpr gpio_num_t MOTOR_RIGHT_SPEED_PIN = GPIO_NUM_2;
        constexpr gpio_num_t MOTOR_RIGHT_DIRECTION_PIN = GPIO_NUM_15;
    
        constexpr ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;
        constexpr ledc_timer_bit_t LEDC_RESOLUTION = LEDC_TIMER_10_BIT;
        constexpr uint32_t LEDC_FREQUENCY = 10000;  // 10 kHz
    
        constexpr uint8_t RAMP_STEP = 20;
        constexpr uint8_t RAMP_INTERVAL_MS = 10;
    }

    namespace Constants {
        constexpr uint16_t MAX_MOTOR_SPEED = 1023;
    }
    
    class Motor {
    private:
        static constexpr const char* TAG = "Libs::Motor";
    
        TaskHandle_t ramp_task_handle;
        
        Boboter::Types::Motor::Id motor_id;
        
        gpio_num_t speed_pin;
        gpio_num_t direction_pin;
        ledc_channel_t ledc_channel;
        
        bool inverse_direction;
        
        uint16_t target_speed;
        uint16_t current_speed;
        Boboter::Types::Motor::Direction current_direction;
    
        static void rampTask(void* params);
    
    public:
        explicit Motor(Boboter::Types::Motor::Id motor_number);
        ~Motor();
    
        void stop(bool wait = true);
        
        void setSpeed(uint16_t speed);
        void setDirection(Boboter::Types::Motor::Direction direction);
    
        uint16_t getSpeed() const { return current_speed; }

        Boboter::Types::Motor::Direction getDirection() const { return current_direction; }
        Boboter::Types::Motor::Direction getActualDirection(Boboter::Types::Motor::Direction) const;
    };
}