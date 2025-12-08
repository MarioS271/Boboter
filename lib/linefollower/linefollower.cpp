/**
 * @file linefollower.cpp
 * @authors MarioS271
 */

#include "linefollower.hpp"
#include "linefollower_types.hpp"

#include "logger.hpp"
#include "error.hpp"

Linefollower::Linefollower()
{
    ERROR_CHECK(TAG, gpio_reset_pin(LF_RIGHT_LEFT_PIN));
    ERROR_CHECK(TAG, gpio_set_direction(LF_RIGHT_LEFT_PIN, GPIO_MODE_OUTPUT));

    adc_unit = ADC_Config::adc_handle;
    cal_handle = ADC_Config::cal_handle;

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_Config::ADC_ATTEN,
        .bitwidth = ADC_Config::ADC_BITWIDTH
    };
    ERROR_CHECK(TAG, adc_oneshot_config_channel(adc_unit, ADC_CHANNEL, &chan_cfg));

    LOGI(TAG, "Initialized Linefollower");
}

lf_result_t Linefollower::get(lf_module_t module)
{
    WARN_CHECK(TAG, gpio_set_level(LF_RIGHT_LEFT_PIN, static_cast<uint32_t>(module)));

    int reading;
    ERROR_CHECK(TAG, adc_oneshot_read(adc_unit, ADC_CHANNEL, &reading));

    lf_result_t result;
    if (reading < 2300)
        result = LF_WHITE;
    else
        result = LF_BLACK;

    return result;
}