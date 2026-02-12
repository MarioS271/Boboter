/**
 * @file adc.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "include/hal/adc.h"

#include <algorithm>
#include <driver/gpio.h>
#include "types/smart_mutex.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace HAL::ADC {
    Controller::Controller() :
        mutex(xSemaphoreCreateMutex()),
        config(),
        is_configured(false)
    {
        LOGI("Constructor of HAL::ADC::Controller called");
    }

    Controller::~Controller() {
        LOGI("Deconstructor of HAL::ADC::Controller called");

        shutdown();

        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
            mutex = nullptr;
        }
    }

    void Controller::configure(const controller_config_t& config) {
        if (is_configured) {
            LOGW("Unable to configure ADC controller, controller is already configured");
            return;
        }

        smart_mutex lock(mutex);

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

        LOGI("Initialized HAL::ADC::Controller");
    }

    void Controller::shutdown() {
        if (!is_configured) {
            LOGW("Unable to shut down ADC controller, controller is not configured");
            return;
        }

        smart_mutex lock(mutex);

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

        LOGI("Shut down HAL::ADC::Controller");
    }

    void Controller::add(const adc_channel_t adc_channel) {
        if (!is_configured) {
            LOGW("Unable to add ADC channel, controller is not configured");
            return;
        }

        smart_mutex lock(mutex);

        const auto adc_channel_number = static_cast<uint8_t>(adc_channel);

        if (std::ranges::contains(registered_channels, adc_channel)) {
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

    uint16_t Controller::read_raw(const adc_channel_t adc_channel, const uint16_t samples) const {
        if (!is_configured) {
            LOGW("Unable to read raw value, controller is not configured (returning zero)");
            return 0;
        }

        smart_mutex lock(mutex);

        if (!std::ranges::contains(registered_channels, adc_channel)) {
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

        if (!is_configured) {
            LOGW("Unable to voltage, controller is not configured (returning zero)");
            return 0;
        }

        smart_mutex lock(mutex);

        if (!std::ranges::contains(registered_channels, adc_channel)) {
            LOGW("Unable to read voltage, channel is not registered (returning zero)");
            return 0;
        }

        if (cali_handle != nullptr) {
            WARN_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_value, &voltage_mv));
        } else {
            LOGW("Unable to convert raw value to voltage due to missing calibration handle, defaulting to linear approximation");

            uint16_t max_voltage;
            switch (config.attenuation) {
                case ADC_ATTEN_DB_0: max_voltage = 1100; break;
                case ADC_ATTEN_DB_2_5: max_voltage = 1500; break;
                case ADC_ATTEN_DB_6: max_voltage = 2200; break;
                case ADC_ATTEN_DB_12: max_voltage = 3300; break;
                default: max_voltage = 1100; break;
            }

            auto bitwidth = static_cast<uint16_t>(config.bitwidth);
            if (bitwidth < 9 || bitwidth > 13) {
                bitwidth = 12;
            }

            const uint32_t scaled_voltage = static_cast<uint32_t>(raw_value) * static_cast<uint32_t>(max_voltage);
            voltage_mv = static_cast<uint16_t>(scaled_voltage / (1 << bitwidth));
        }

        return static_cast<uint16_t>(voltage_mv);
    }
}