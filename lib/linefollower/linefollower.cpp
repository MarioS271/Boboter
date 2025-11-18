// LINEFOLLOWER_CPP
// Boboter
// (C) MarioS271 2025

#include "linefollower.h"
#include "linefollower_types.h"

#include "delay.h"
#include "logger.h"
#include "esp_err.h"

Linefollower::Linefollower() {
    ESP_ERROR_CHECK(gpio_set_direction(LF_RIGHT_LEFT_PIN, GPIO_MODE_OUTPUT));

    adc_unit = ADC_CONFIG::adc_handle;
    cal_handle = ADC_CONFIG::cal_handle;

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_CONFIG::ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_unit, ADC_CHANNEL, &chan_cfg));

    LOGI(TAG, "Initialized Linefollower");
}

lf_result_t Linefollower::get(lf_module_t module) {
    gpio_set_level(LF_RIGHT_LEFT_PIN, module);
    delay(1);

    int reading;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_unit, ADC_CHANNEL, &reading));

    LOGI(TAG, "adc reading (%s): %d", module == LF_LEFT ? "LF_LEFT" : "LF_RIGHT", reading);

    lf_result_t result;
    if (reading < 2047) result = LF_WHITE;
    else result = LF_BLACK;

    return result;
}