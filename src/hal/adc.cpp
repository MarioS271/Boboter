/**
 * @file adc.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/hal/adc.h"

#include <driver/gpio.h>
#include "helpers/gpio_adc_conversions.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace ADC {
    Controller::Controller() {
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

        is_configured = false;
        LOGI("Shut down ADC controller HAL");
    }

    void Controller::add(const std::optional<adc_channel_t> adc_channel) {
        if (!is_configured) {
            LOGW("Unable to add ADC_CHANNEL_%d because ADC controller is not registered");
            return;
        }


    }

    void Controller::remove(const std::optional<adc_channel_t> adc_channel) {

    }

    bool Controller::is_registered(const std::optional<adc_channel_t> adc_channel) const {

    }

    uint16_t Controller::read_raw(const std::optional<adc_channel_t> adc_channel) const {

    }

    uint16_t Controller::read_millivolts(const std::optional<adc_channel_t> adc_channel) const {
    }
}