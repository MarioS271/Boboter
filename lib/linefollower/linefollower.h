// LINEFOLLOWER_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "linefollower_types.h"

#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include "adc_utils.h"

class Linefollower {
private:
    static constexpr const char* TAG = "LINEFOLLOWER";

    static constexpr gpio_num_t LF_RIGHT_LEFT_PIN = GPIO_NUM_12;
    static constexpr adc_channel_t ADC_CHANNEL = ADC_CHANNEL_0;

    adc_oneshot_unit_handle_t adc_unit = nullptr;
    adc_cali_handle_t cal_handle = nullptr;

public:
    Linefollower();
    ~Linefollower() = default;

    lf_result_t get(lf_module_t module);
};