/**
 * @file adc.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/hal/adc.h"

#include <array>
#include <algorithm>
#include <driver/gpio.h>
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace ADC {
    Controller::Controller() :
        config(),
        is_configured(false)
    {
        LOGI("Constructor of ADC::Controller called");
    }

    Controller::~Controller() {
        LOGI("Deconstructor of ADC::Controller called");
        shutdown();
    }

    void Controller::configure(const config_t& config) {
        const adc_oneshot_unit_init_cfg_t unit_config = {
            .unit_id = ADC_UNIT_1,
            .clk_src = config.clock_source,
            .ulp_mode = config.ulp_mode
        };
        ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &adc_handle));

        const adc_cali_line_fitting_config_t cali_config = {
            .unit_id = ADC_UNIT_1,
            .atten = config.attenuation,
            .bitwidth = config.bitwidth
        };
        WARN_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle));

        this->config = config;
        is_configured = true;

        LOGI("Initialized ADC::Controller HAL");
    }

    void Controller::shutdown() {
        if (adc_handle != nullptr) {
            WARN_CHECK(adc_oneshot_del_unit(adc_handle));
            adc_handle = nullptr;
        }

        if (cali_handle != nullptr) {
            WARN_CHECK(adc_cali_delete_scheme_line_fitting(cali_handle));
            cali_handle = nullptr;
        }

        if (!registered_channels.empty()) {
            for (const auto& channel : registered_channels) {
                gpio_reset_pin(adc_channel_to_gpio(channel));
            }
            registered_channels.clear();
        }

        config = {};
        is_configured = false;

        LOGI("Shut down the ADC controller HAL");
    }

    void Controller::add(const adc_channel_t adc_channel) {
        if (!is_configured) {
            LOGW("Unable to add ADC channel because ADC controller is not registered");
            return;
        }

        const auto adc_channel_number = static_cast<uint8_t>(adc_channel);

        if (is_registered(adc_channel)) {
            LOGW("ADC channel %d is already registered", adc_channel_number);
            return;
        }

        const adc_oneshot_chan_cfg_t channel_config = {
            .atten = config.attenuation,
            .bitwidth = config.bitwidth
        };

        ERROR_CHECK(adc_oneshot_config_channel(adc_handle, adc_channel, &channel_config));

        registered_channels.push_back(adc_channel);
        LOGI("Initialized ADC channel %d of ADC1", adc_channel_number);
    }

    void Controller::remove(const adc_channel_t adc_channel) {
        if (!is_configured) {
            LOGW("Unable to remove ADC channel because ADC controller is not registered");
            return;
        }

        const auto adc_channel_number = static_cast<uint8_t>(adc_channel);

        WARN_CHECK(gpio_reset_pin(adc_channel_to_gpio(adc_channel)));

        if (const auto channel = std::ranges::find(registered_channels, adc_channel);
            channel != registered_channels.end())
        {
            registered_channels.erase(channel);
        }

        LOGI("Removed ADC channel %d of ADC1", adc_channel_number);
    }

    bool Controller::is_registered(const adc_channel_t adc_channel) const {
        return std::ranges::contains(registered_channels, adc_channel);
    }

    uint16_t Controller::read_raw(const adc_channel_t adc_channel, const uint16_t samples) const {
        if (!is_registered(adc_channel)) {
            LOGW("Unable to read raw value, channel is not registered (returning zero)");
            return 0;
        }

        if (samples == 0) {
            LOGE("Unable to read raw value, illegal number of samples given");
            abort();
        }

        int raw_value = 0;
        uint32_t average_value = 0;

        for (uint16_t i = 0; i < samples; ++i) {
            WARN_CHECK(adc_oneshot_read(adc_handle, adc_channel, &raw_value));
            average_value += raw_value;
        }

        average_value /= samples;

        return static_cast<uint16_t>(average_value);
    }

    uint16_t Controller::read_millivolts(const adc_channel_t adc_channel, const uint16_t samples) const {
        const uint16_t raw_value = read_raw(adc_channel, samples);
        int voltage_mv = 0;

        if (cali_handle != nullptr) {
            WARN_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_value, &voltage_mv));
        } else {
            LOGW("Unable to convert raw value to voltage due to missing calibration handle, defaulting to linear approximation");

            // For the linear approximation, a max voltage of 3.3V (3300)
            // and a resolution of 12 bits (2^12 or 4095) are used
            voltage_mv = raw_value * 3300 / 4095;
        }

        return static_cast<uint16_t>(voltage_mv);
    }
}