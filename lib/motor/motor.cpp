/**
 * @file motor.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "motor.hpp"

#include "helpers/delay.hpp"
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Motor {
    Motor::Motor(Boboter::Types::Motor::Id motor_id, bool inverse_direction)
    : ramp_task_handle(nullptr),
      motor_id(motor_id),
      inverse_direction(inverse_direction),
      target_speed(0),
      current_speed(0),
      current_direction(Boboter::Types::Motor::Direction::FORWARD)
    {
        using namespace Config;
        using namespace Boboter::Types::Motor;
        using Boboter::Helpers::delay;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        switch (motor_id) {
            case Id::LEFT:
                LOGI(TAG, "Initialized MOTOR_LEFT (ID: %d)", motor_id);
                speed_pin = MOTOR_LEFT_SPEED_PIN;
                direction_pin = MOTOR_LEFT_DIRECTION_PIN;
                ledc_channel = LEDC_CHANNEL_0;
                break;
    
            case Id::RIGHT:
                LOGI(TAG, "Initialized MOTOR_RIGHT (ID: %d)", motor_id);
                speed_pin = MOTOR_RIGHT_SPEED_PIN;
                direction_pin = MOTOR_RIGHT_DIRECTION_PIN;
                ledc_channel = LEDC_CHANNEL_1;
                ramp_task_name = "MotorRightRampTask";
                break;
    
            default:
                LOGE(TAG, "Unable to initialize Motor (ID: %d)", motor_id);
                abort();
        }
    
        gpio_config_t config = {
            .pin_bit_mask = (1ull << direction_pin),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
        ERROR_CHECK(TAG, gpio_reset_pin(direction_pin));
        ERROR_CHECK(TAG, gpio_config(&config));
    
        ledc_timer_config_t ledc_timer_conf = {};
        ledc_timer_conf.speed_mode = LEDC_SPEED_MODE;
        ledc_timer_conf.timer_num = LEDC_TIMER_0;
        ledc_timer_conf.duty_resolution = LEDC_RESOLUTION;
        ledc_timer_conf.freq_hz = LEDC_FREQUENCY;
        ledc_timer_conf.clk_cfg = LEDC_AUTO_CLK;
        ERROR_CHECK(TAG, ledc_timer_config(&ledc_timer_conf));
        
        ledc_channel_config_t ledc_channel_conf = {};
        ledc_channel_conf.gpio_num = speed_pin;
        ledc_channel_conf.speed_mode = LEDC_SPEED_MODE;
        ledc_channel_conf.channel = ledc_channel;
        ledc_channel_conf.timer_sel = LEDC_TIMER_0;
        ledc_channel_conf.duty = 0;
        ledc_channel_conf.hpoint = 0;
        ERROR_CHECK(TAG, ledc_channel_config(&ledc_channel_conf));
    
        delay(10);
        xTaskCreate(
            Internal::ramp_task,
            ramp_task_name.c_str(),
            RAMP_TASK_STACK_DEPTH,
            this,
            RAMP_TASK_PRIORITY,
            &ramp_task_handle
        );
    }
    
    Motor::~Motor() {
        if (ramp_task_handle != nullptr) {
            vTaskDelete(ramp_task_handle);
            ramp_task_handle = nullptr;
        }
        stop(true);
    }
    
    void Motor::stop(bool wait) {
        using namespace Config;
        using Boboter::Helpers::delay;
        using namespace Boboter::Libs::Error;
    
        target_speed = 0;
    
        if (wait) {
            const uint32_t timeout_ms = 2000;
            uint32_t waited_ms = 0;
    
            while (current_speed > RAMP_STEP && waited_ms < timeout_ms) {
                delay(RAMP_INTERVAL_MS);
                waited_ms += RAMP_INTERVAL_MS;
            }
    
            WARN_CHECK(TAG, ledc_set_duty(LEDC_SPEED_MODE, ledc_channel, 0));
            WARN_CHECK(TAG, ledc_update_duty(LEDC_SPEED_MODE, ledc_channel));
    
            current_speed = 0;
        }
    }
    
    void Motor::set_speed(uint16_t speed) {
        using namespace Boboter::Libs::Motor::Constants;

        if (speed > MAX_MOTOR_SPEED)
            speed = MAX_MOTOR_SPEED;
    
        target_speed = speed;
    }
    
    void Motor::set_direction(Boboter::Types::Motor::Direction direction) {
        using namespace Boboter::Types::Motor;
        using namespace Boboter::Libs::Error;
        
        WARN_CHECK(TAG, gpio_set_level(
            direction_pin,
            get_actual_direction(direction) == Direction::FORWARD ? 1 : 0
        ));
        current_direction = direction;
    }
}