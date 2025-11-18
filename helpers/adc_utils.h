// ADC_UTILS_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

namespace ADC_CONFIG {
    constexpr adc_unit_t ADC_UNIT = ADC_UNIT_1;
    constexpr adc_atten_t ADC_ATTEN = ADC_ATTEN_DB_12;
    constexpr adc_bitwidth_t ADC_BITWIDTH = ADC_BITWIDTH_DEFAULT;
    constexpr adc_ulp_mode_t ADC_ULP_MODE = ADC_ULP_MODE_DISABLE;
    constexpr adc_oneshot_clk_src_t ADC_CLK_SRC = ADC_RTC_CLK_SRC_DEFAULT;

    inline adc_oneshot_unit_handle_t adc_handle = nullptr;
    inline adc_cali_handle_t cal_handle = nullptr;
}

inline void init_adc() {
    using namespace ADC_CONFIG;

    if (adc_handle != nullptr) return;

    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT,
        .clk_src = ADC_CLK_SRC,
        .ulp_mode = ADC_ULP_MODE
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc_handle));

    adc_cali_line_fitting_config_t cal_cfg = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH,
        .default_vref = 0
    };
    if (adc_cali_create_scheme_line_fitting(&cal_cfg, &cal_handle) != ESP_OK) {
        cal_handle = nullptr;
    }
}