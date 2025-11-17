// CHECK_BATTERY_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <rom/ets_sys.h>
#include "map.h"

namespace CHECK_BATTERY_CONFIG {
    static constexpr adc1_channel_t BAT_ADC_CHANNEL = ADC1_CHANNEL_3;
    static constexpr adc_atten_t BAT_ADC_ATTEN = ADC_ATTEN_DB_12;
    static constexpr uint32_t DEFAULT_VREF = 1100;  // mV
    static constexpr uint8_t NUM_SAMPLES = 16;
    static esp_adc_cal_characteristics_t adc_chars;
}


inline void initialize_battery_checker() {
    using namespace CHECK_BATTERY_CONFIG;
    
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(BAT_ADC_CHANNEL, BAT_ADC_ATTEN);
    esp_adc_cal_characterize(ADC_UNIT_1, BAT_ADC_ATTEN, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);
}

void check_battery_percentage(uint8_t* percentage, uint16_t* voltage_mv) {
    using namespace CHECK_BATTERY_CONFIG;

    uint32_t sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += adc1_get_raw(BAT_ADC_CHANNEL);
        ets_delay_us(100);
    }

    uint32_t avg_raw = sum / NUM_SAMPLES;
    uint32_t mv_adc = esp_adc_cal_raw_to_voltage(avg_raw, &adc_chars);

    uint16_t mv = static_cast<uint16_t>((mv_adc * 1470) / 1000);

    if (voltage_mv != nullptr) {
        *voltage_mv = mv;
    }

    if (percentage != nullptr) {
        int percent = map_value(mv, 3000, 4200, 0, 100);

        if (percent > 100) percent = 100;
        if (percent < 0)   percent = 0;

        *percentage = static_cast<uint8_t>(percent);
    }
}