/**
 * @file motor.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "motor_types.hpp"

#include <string>
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

        constexpr uint8_t RAMP_TASK_PRIORITY = 14;
        constexpr uint16_t RAMP_TASK_STACK_DEPTH = 1024;
    }

    namespace Constants {
        constexpr uint16_t MAX_MOTOR_SPEED = 1023;
    }

    namespace Internal {
        void ramp_task(void* params);
    }
    
    class Motor {
    private:
        static constexpr const char* TAG = "Libs::Motor";
    
        TaskHandle_t ramp_task_handle;
        std::string ramp_task_name;
        Boboter::Types::Motor::Id motor_id;
        
        gpio_num_t speed_pin;
        gpio_num_t direction_pin;
        ledc_channel_t ledc_channel;
        
        bool inverse_direction;
        
        uint16_t target_speed;
        uint16_t current_speed;
        Boboter::Types::Motor::Direction current_direction;

        friend void Internal::ramp_task(void*);
    
    public:
        explicit Motor(Boboter::Types::Motor::Id motor_id, bool inverse_direction);
        ~Motor();
    
        void stop(bool wait = true);
        
        void set_speed(uint16_t speed);
        void set_direction(Boboter::Types::Motor::Direction direction);
    
        uint16_t get_speed() const { return current_speed; }
        Boboter::Types::Motor::Direction get_direction() const { return current_direction; }

        Boboter::Types::Motor::Direction get_actual_direction(Boboter::Types::Motor::Direction apparent_direction) const
        { return static_cast<Boboter::Types::Motor::Direction>(apparent_direction ^ inverse_direction); }
    };
}