/**
 * @file adc.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <optional>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>

namespace ADC {
    struct Config {
        adc_atten_t attenuation;
        adc_bitwidth_t bitwidth;
        adc_ulp_mode_t ulp_mode = ADC_ULP_MODE_DISABLE;
        adc_oneshot_clk_src_t clock_source = ADC_RTC_CLK_SRC_DEFAULT;
    };

    class Controller {
    private:
        static constexpr const char* TAG = "HAL:ADC";

        Config config;
        adc_oneshot_unit_handle_t adc_handle = nullptr;
        adc_cali_handle_t cali_handle = nullptr;
        bool is_configured = false;

        std::vector<adc_channel_t> registered_channels;

    private:
        explicit Controller();
        ~Controller();

        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

    public:
        /**
         * @brief Returns a reference to the static controller instance
         * @note Controller instance will be created on first call of this function
         *
         * @return Reference to the controller object
         */
        static Controller& get_instance() {
            static Controller _instance;
            return _instance;
        }

        /**
         * @brief Configure the ADC Controller
         * @attention On illegal config, firmware will abort
         *
         * @param config The config struct to use
         */
        void configure(const Config& config);

        /**
         * @brief Safely shuts down the controller and hardware
         */
        void shutdown();

        /**
         * @brief Adds an ADC input to the controller
         *
         * @param adc_channel The ADC channel to add
         */
        void add(std::optional<adc_channel_t> adc_channel);

        /**
         * @brief Removes an ADC input from the controller
         * @note If @c std::nullopt is recieved, firmware will abort
         *
         * @param adc_channel The ADC channel to remove
         */
        void remove(std::optional<adc_channel_t> adc_channel);

        /**
         * @brief Checks if a specific ADC channel is registered in the controller
         * @note If @c std::nullopt is recieved, firmware will warn and continue
         *
         * @return Whether the pin is registered (true) or not (false)
         *
         * @param adc_channel The ADC channel to check for
         */
        [[nodiscard]] bool is_registered(std::optional<adc_channel_t> adc_channel) const;

        /**
         * @brief Reads the current raw ADC reading
         * @attention If @c std::nullopt is recieved, firmware will abort
         *
         * @return The raw ADC reading
         *
         * @param adc_channel The ADC channel to read from
         * @param samples The amount of samples to take when reading (default: 1)
         */
        [[nodiscard]] uint16_t read_raw(std::optional<adc_channel_t> adc_channel, uint16_t samples = 1) const;

        /**
         * @brief Reads the current voltage in millivolts
         * @attention If @c std::nullopt is recieved, firmware will abort
         *
         * @return The voltage in millivolts
         * 
         * @param adc_channel The ADC channel to read from
         * @param samples The amount of samples to take when reading (default: 1)
         */
        [[nodiscard]] uint16_t read_millivolts(std::optional<adc_channel_t> adc_channel, uint16_t samples = 1) const;
    };
}