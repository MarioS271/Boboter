/**
 * @file battery.cpp
 * @authors MarioS271
 */

#include "battery.hpp"

#include <rom/ets_sys.h>
#include "logger.hpp"
#include "error.hpp"
#include "map.hpp"

BatteryManager::BatteryManager()
{
    ERROR_CHECK(TAG, gpio_reset_pin(ADC_GPIO));

    adc_unit = ADC_Config::adc_handle;
    cal_handle = ADC_Config::cal_handle;

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_Config::ADC_ATTEN,
        .bitwidth = ADC_Config::ADC_BITWIDTH
    };
    ERROR_CHECK(TAG, adc_oneshot_config_channel(adc_unit, ADC_CHANNEL, &chan_cfg));

    LOGI(TAG, "Initialized Battery Manager");
}

void BatteryManager::update()
{
    uint32_t sum = 0;
    
    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        int raw = 0;
        ERROR_CHECK(TAG, adc_oneshot_read(adc_unit, ADC_CHANNEL, &raw));
        sum += raw;
        ets_delay_us(500);
    }

    uint32_t avg_raw = sum / NUM_SAMPLES;
    int mv_adc = 0;

    if (cal_handle)
        ERROR_CHECK(TAG, adc_cali_raw_to_voltage(cal_handle, avg_raw, &mv_adc));
    else
        mv_adc = (avg_raw * 3300) / 4095;

    voltage_mv = static_cast<uint16_t>((mv_adc * 1470) / 1000);

    uint8_t pct = map_value(voltage_mv, 3300, 4200, 0, 100);
    if (pct > 100) pct = 100;
    percentage = pct;
}