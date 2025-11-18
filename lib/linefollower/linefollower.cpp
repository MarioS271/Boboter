// LINEFOLLOWER_CPP
// Boboter
// (C) MarioS271 2025

#include "linefollower.h"
#include "linefollower_types.h"

#include "delay.h"
#include "logger.h"
#include "error.h"
// #include "rom/ets_sys.h"

Linefollower::Linefollower() {
    ERROR_CHECK(TAG, gpio_set_direction(LF_RIGHT_LEFT_PIN, GPIO_MODE_OUTPUT));
    ERROR_CHECK(TAG, gpio_set_pull_mode(LF_RIGHT_LEFT_PIN, GPIO_FLOATING));

    adc_unit = ADC_CONFIG::adc_handle;
    cal_handle = ADC_CONFIG::cal_handle;

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_CONFIG::ADC_ATTEN,
        .bitwidth = ADC_CONFIG::ADC_BITWIDTH
    };
    ERROR_CHECK(TAG, adc_oneshot_config_channel(adc_unit, ADC_CHANNEL, &chan_cfg));

    LOGI(TAG, "Initialized Linefollower");
}

lf_result_t Linefollower::get(lf_module_t module) {
    WARN_CHECK(TAG, gpio_set_level(LF_RIGHT_LEFT_PIN, static_cast<uint32_t>(module)));
    delay(5);
    
    int reading;
    ERROR_CHECK(TAG, adc_oneshot_read(adc_unit, ADC_CHANNEL, &reading));

    LOGI(TAG, "adc reading (%s, %d): %d", module == LF_LEFT ? "LF_LEFT" : "LF_RIGHT", gpio_get_level(LF_RIGHT_LEFT_PIN), reading);

    lf_result_t result;
    if (reading < 2047) result = LF_WHITE;
    else result = LF_BLACK;

    return result;
}