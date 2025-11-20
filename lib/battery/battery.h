// BATTERY_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include "adc_utils.h"

class BatteryManager {
private:
    static constexpr const char* TAG = "BATTERY_MANAGER";
    
    static constexpr gpio_num_t ADC_GPIO = GPIO_NUM_39;
    static constexpr adc_channel_t ADC_CHANNEL = ADC_CHANNEL_3;
    static constexpr uint8_t NUM_SAMPLES = 16;

    adc_oneshot_unit_handle_t adc_unit = nullptr;
    adc_cali_handle_t cal_handle = nullptr;

    uint16_t voltage_mv = 0;
    uint8_t percentage = 0;

public:
    BatteryManager();
    ~BatteryManager() = default;

    void update();
    uint8_t getPercentage() const { return percentage; };
    uint16_t getVoltageMv() const { return voltage_mv; };
};