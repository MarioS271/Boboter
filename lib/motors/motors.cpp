// MOTORS_CPP
// Boboter
// (C) MarioS271 2025

#include "motors.h"

#include "esp_log.h"

static const char* TAG = "class_motor";

// Constructor
Motor::Motor(motor_num_t motor_number) : motor_num(motor_number) {
    error = false;

    // Assign correct pin numbers
    switch (motor_num) {
        case MOTOR_LEFT:
            ESP_LOGI(TAG, "Initialized Motor MOTOR_LEFT (ID: %d)", motor_num);
            speed_pin = MOTOR_LEFT_SPEED_PIN;
            direction_pin = MOTOR_LEFT_DIRECTION_PIN;
            ledc_channel = LEDC_CHANNEL_0;
            inverse_direction = true;
            break;

        case MOTOR_RIGHT:
            ESP_LOGI(TAG, "Initialized Motor MOTOR_RIGHT (ID: %d)", motor_num);
            speed_pin = MOTOR_RIGHT_SPEED_PIN;
            direction_pin = MOTOR_RIGHT_DIRECTION_PIN;
            ledc_channel = LEDC_CHANNEL_1;
            inverse_direction = false;
            break;

        default:
            ESP_LOGE(TAG, "Unable to initialize Motor (ID: %d)", motor_num);
            inverse_direction = false;
            error = true;
            return;
    }

    // Direction Pin Config
    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = (1ULL << direction_pin);
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_conf);

    // Speed Pin Config
    ledc_timer_config_t ledc_timer_conf = {};
    ledc_timer_conf.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer_conf.timer_num = LEDC_TIMER_0;
    ledc_timer_conf.duty_resolution = LEDC_RESOLUTION;
    ledc_timer_conf.freq_hz = LEDC_FREQUENCY;
    ledc_timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer_conf);

    ledc_channel_config_t ledc_channel_conf = {};
    ledc_channel_conf.gpio_num = speed_pin;
    ledc_channel_conf.speed_mode = LEDC_SPEED_MODE;
    ledc_channel_conf.channel = ledc_channel;
    ledc_channel_conf.timer_sel = LEDC_TIMER_0;
    ledc_channel_conf.duty = 0;
    ledc_channel_conf.hpoint = 0;
    ledc_channel_config(&ledc_channel_conf);
}



// Private Helper: Apply Direction Inverse Factor
motor_direction_t Motor::getActualDirection(motor_direction_t apparent_direction) {
    if (inverse_direction) {
        return apparent_direction == FORWARD ? BACKWARD : FORWARD;
    }
    return apparent_direction;
}



// Stop Function
void Motor::stop() {
    setSpeed(0);

    ESP_LOGI(TAG, "Motor %d stopped", motor_num);
}

// Set Speed Function
void Motor::setSpeed(uint16_t speed) {
    speed = speed > 1023 ? 1023 : speed;

    ledc_set_duty(LEDC_SPEED_MODE, ledc_channel, speed);
    ledc_update_duty(LEDC_SPEED_MODE, ledc_channel);

    saved_speed = speed;
    ESP_LOGI(TAG, "Motor %d speed set to %d", motor_num, speed);
}

// Set Direction Function
void Motor::setDirection(motor_direction_t direction) {
    gpio_set_level(direction_pin, getActualDirection(direction) == FORWARD ? 1 : 0);

    saved_direction = direction;
    ESP_LOGI(TAG, "Motor %d direction set to %s", motor_num,
             direction == FORWARD ? "FORWARD" : "BACKWARD");
}



// Get Speed Function
uint16_t Motor::getSpeed() {
    return saved_speed;
}

// Get Direction Function
motor_direction_t Motor::getDirection() {
    return saved_direction;
}