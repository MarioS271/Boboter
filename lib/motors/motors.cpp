// MOTORS_CPP
// Boboter
// (C) MarioS271 2025

#include "motors.h"

#include "delay.h"
#include "logger.h"
#include "esp_err.h"

#define TAG "MOTORS"

Motor::Motor(motor_num_t motor_number)
: target_speed(0),
  rampTaskHandle(nullptr),
  motor_num(motor_number),
  error(false),
  current_speed(0),
  current_direction(M_FORWARD)
{
    switch (motor_num) {
        case MOTOR_LEFT:
            LOGI(TAG, "Initialized Motor MOTOR_LEFT (ID: %d)", motor_num);
            speed_pin = MOTOR_LEFT_SPEED_PIN;
            direction_pin = MOTOR_LEFT_DIRECTION_PIN;
            ledc_channel = LEDC_CHANNEL_0;
            inverse_direction = true;
            break;

        case MOTOR_RIGHT:
            LOGI(TAG, "Initialized Motor MOTOR_RIGHT (ID: %d)", motor_num);
            speed_pin = MOTOR_RIGHT_SPEED_PIN;
            direction_pin = MOTOR_RIGHT_DIRECTION_PIN;
            ledc_channel = LEDC_CHANNEL_1;
            inverse_direction = false;
            break;

        default:
            LOGE(TAG, "Unable to initialize Motor (ID: %d)", motor_num);
            inverse_direction = false;
            error = true;
            return;
    }

    ESP_ERROR_CHECK(gpio_set_direction(direction_pin, GPIO_MODE_OUTPUT));

    ledc_timer_config_t ledc_timer_conf = {};
    ledc_timer_conf.speed_mode = LEDC_SPEED_MODE;
    ledc_timer_conf.timer_num = LEDC_TIMER_0;
    ledc_timer_conf.duty_resolution = LEDC_RESOLUTION;
    ledc_timer_conf.freq_hz = LEDC_FREQUENCY;
    ledc_timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_conf));
    
    ledc_channel_config_t ledc_channel_conf = {};
    ledc_channel_conf.gpio_num = speed_pin;
    ledc_channel_conf.speed_mode = LEDC_SPEED_MODE;
    ledc_channel_conf.channel = ledc_channel;
    ledc_channel_conf.timer_sel = LEDC_TIMER_0;
    ledc_channel_conf.duty = 0;
    ledc_channel_conf.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_conf));

    delay(10);
    xTaskCreate(rampTask, "MotorRampTask", 2048, this, 5, &rampTaskHandle);
}
Motor::~Motor() {
    if (rampTaskHandle != nullptr) {
        vTaskDelete(rampTaskHandle);
        rampTaskHandle = nullptr;
    }
    stop(true);
}


void Motor::stop(bool wait) {
    if (error) return;

    target_speed = 0;

    if (wait) {
        const uint32_t timeout_ms = 2000;
        uint32_t waited_ms = 0;

        while (current_speed > RAMP_STEP && waited_ms < timeout_ms) {
            delay(RAMP_INTERVAL_MS);
            waited_ms += RAMP_INTERVAL_MS;
        }

        esp_err_t duty_set_ret = ledc_set_duty(LEDC_SPEED_MODE, ledc_channel, 0);
        esp_err_t duty_update_ret = ledc_update_duty(LEDC_SPEED_MODE, ledc_channel);

        if (duty_set_ret != ESP_OK || duty_update_ret != ESP_OK) {
            LOGE(TAG, "ledc_set_duty or ledc_update_duty failed");
            return;
        }

        current_speed = 0;
    }
}

void Motor::setSpeed(uint16_t speed) {
    if (error) return;

    speed = speed > 1023 ? 1023 : speed;
    target_speed = speed;
}

void Motor::setDirection(motor_direction_t direction) {
    if (error) return;
    
    gpio_set_level(direction_pin, getActualDirection(direction) == M_FORWARD ? 1 : 0);
    current_direction = direction;
}

motor_direction_t Motor::getActualDirection(motor_direction_t apparent_direction) const {
    if (inverse_direction) {
        return apparent_direction == M_FORWARD ? M_BACKWARD : M_FORWARD;
    }
    return apparent_direction;
}