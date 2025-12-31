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
#include "helpers/gpio_adc_conversions.h"
#include "helpers/check_for_nullopt.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace ADC {
    Controller::Controller() : config() {
        LOGI("Constructor of ADC::Controller called");
    }

    Controller::~Controller() {
        LOGI("Deconstructor of ADC::Controller called");
        shutdown();
    }

    void Controller::configure(const Config& config) {
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
                std::optional<gpio_num_t> pin = adc_channel_to_gpio(channel);
                if (pin.has_value())
                    gpio_reset_pin(pin.value());
            }
        }

        registered_channels.clear();
        this->config = {};
        is_configured = false;

        LOGI("Shut down ADC controller HAL");
    }

    void Controller::add(const std::optional<adc_channel_t> adc_channel) {
        CHECK_FOR_NULLOPT(adc_channel);

        if (!is_configured) {
            LOGW("Unable to add ADC channel because ADC controller is not registered");
            return;
        }

        const adc_channel_t adc_channel_value = adc_channel.value();
        const uint8_t adc_channel_number = static_cast<uint8_t>(adc_channel_value);

        if (is_registered(adc_channel)) {
            LOGW("ADC channel %d is already registered", adc_channel_number);
            return;
        }

        const adc_oneshot_chan_cfg_t channel_config = {
            .atten = config.attenuation,
            .bitwidth = config.bitwidth
        };

        const esp_err_t result = adc_oneshot_config_channel(adc_handle, adc_channel.value(), &channel_config);
        WARN_CHECK(result);

        if (result == ESP_OK) {
            registered_channels.push_back(adc_channel.value());
            LOGI("Initialized ADC channel %d of ADC1", adc_channel_number);
        } else {
            LOGW("Unable to initialize ADC channel %d of ADC1", adc_channel_number);
        }
    }

    void Controller::remove(const std::optional<adc_channel_t> adc_channel) {
        CHECK_FOR_NULLOPT(adc_channel);

        if (!is_configured) {
            LOGW("Unable to remove ADC channel because ADC controller is not registered");
            return;
        }

        const adc_channel_t adc_channel_value = adc_channel.value();
        const uint8_t adc_channel_number = static_cast<uint8_t>(adc_channel_value);
        const gpio_num_t gpio_pin = adc_channel_to_gpio(adc_channel_value).value();

        const esp_err_t result = gpio_reset_pin(gpio_pin);
        WARN_CHECK(result);

        if (result == ESP_OK) {
            if (const auto it = std::ranges::find(registered_channels, adc_channel_value);
                it != registered_channels.end())
            {
                registered_channels.erase(it);
            }

            LOGI("Removed ADC channel %d of ADC1", adc_channel_number);
        } else {
            LOGW("Unable to remove ADC channel %d of ADC1", adc_channel_number);
        }
    }

    bool Controller::is_registered(const std::optional<adc_channel_t> adc_channel) const {
        CHECK_FOR_NULLOPT(adc_channel);

        const bool found = std::ranges::contains(registered_channels, adc_channel.value());
        return found;
    }

    uint16_t Controller::read_raw(const std::optional<adc_channel_t> adc_channel, uint16_t samples) const {
        CHECK_FOR_NULLOPT(adc_channel);

        if (!is_registered(adc_channel)) {
            LOGW("Unable to read raw value, channel is not registered (returning zero)");
            return 0;
        }

        int raw_value;
        uint32_t average_value = 0;

        for (uint16_t i = 0; i < samples; ++i) {
            WARN_CHECK(adc_oneshot_read(adc_handle, adc_channel.value(), &raw_value));
            average_value += raw_value;
        }

        average_value /= samples;

        return static_cast<uint16_t>(average_value);
    }

    uint16_t Controller::read_millivolts(const std::optional<adc_channel_t> adc_channel, uint16_t samples) const {
        CHECK_FOR_NULLOPT(adc_channel);

        uint16_t raw_value = read_raw(adc_channel, samples);
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