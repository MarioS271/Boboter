// ENCODER_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "driver/gpio.h"
#include "esp_attr.h"

enum encoder_num_t {
    ENCODER_LEFT = 0,
    ENCODER_RIGHT = 1
};

class Encoder {
    friend void IRAM_ATTR encoder_isr_handler(void* arg);
private:
    static constexpr gpio_num_t ENCODER_LEFT_PIN = GPIO_NUM_14;
    static constexpr gpio_num_t ENCODER_RIGHT_PIN = GPIO_NUM_27;

    encoder_num_t encoder_num;
    gpio_num_t encoder_pin;
    bool error;

    volatile int32_t pulse_count;

public:
    explicit Encoder(encoder_num_t encoder_number);

    int32_t getPulseCount();
    void resetPulseCount();
    
    bool hasError();
};