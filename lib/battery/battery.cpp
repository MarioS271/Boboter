// BATTERY_CPP
// Boboter
// (C) MarioS271 2025

#include "battery.h"

#include "rom/ets_sys.h"
#include "map.h"

BatteryManager::BatteryManager() {
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_unit));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_unit, ADC_CHANNEL, &chan_cfg));

    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id = ADC_UNIT,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (adc_cali_create_scheme_line_fitting(&cali_cfg, &cal_handle) != ESP_OK) {
        cal_handle = nullptr;
    }
}

BatteryManager::~BatteryManager() {
    if (adc_unit) adc_oneshot_del_unit(adc_unit);
    if (cal_handle) adc_cali_delete_scheme_line_fitting(cal_handle);
}

void BatteryManager::update() {
    uint32_t sum = 0;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int raw = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc_unit, ADC_CHANNEL, &raw));
        sum += raw;
        ets_delay_us(500);
    }

    uint32_t avg_raw = sum / NUM_SAMPLES;
    int mv_adc = 0;

    if (cal_handle) ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cal_handle, avg_raw, &mv_adc));
    else mv_adc = (avg_raw * 3300) / 4095;

    voltage_mv = static_cast<uint16_t>((mv_adc * 1470) / 1000);

    uint8_t pct = map_value(voltage_mv, 3300, 4200, 0, 100);
    if (pct > 100) pct = 100;
    if (pct < 0) pct = 0;
    percentage = pct;
}