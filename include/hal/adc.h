/**
 * @file adc.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>

namespace ADC {
    struct controller_config_t {
        adc_atten_t attenuation;
        adc_bitwidth_t bitwidth;
        adc_ulp_mode_t ulp_mode = ADC_ULP_MODE_DISABLE;
        adc_oneshot_clk_src_t clock_source = ADC_RTC_CLK_SRC_DEFAULT;
    };

    class Controller {
    private:
        static constexpr const char* TAG = "HAL:ADC";

        controller_config_t config;
        bool is_configured;

        adc_oneshot_unit_handle_t adc_handle = nullptr;
        adc_cali_handle_t cali_handle = nullptr;

        std::vector<adc_channel_t> registered_channels;

    private:
        explicit Controller();
        ~Controller();

        /**
         * @brief Converts an @c adc_channel_t to a @c gpio_num_t for ADC1
         *
         * @return The converted GPIO pin, or @c GPIO_NUM_NC if the given channel was invalid
         *
         * @param adc_channel The ADC channel to convert
         */
        static gpio_num_t adc_channel_to_gpio(const adc_channel_t adc_channel) {
            switch (adc_channel) {
                case ADC_CHANNEL_0: return GPIO_NUM_36;
                case ADC_CHANNEL_3: return GPIO_NUM_39;
                case ADC_CHANNEL_4: return GPIO_NUM_32;
                case ADC_CHANNEL_5: return GPIO_NUM_33;
                case ADC_CHANNEL_6: return GPIO_NUM_34;
                case ADC_CHANNEL_7: return GPIO_NUM_35;
                default: return GPIO_NUM_NC;
            }
        }

    public:
        /**
         * @brief Returns a reference to the static controller instance
         * @note The instance will be created on the first call of this function
         *
         * @return Reference to the controller object
         */
        static Controller& get_instance() {
            static Controller _instance;
            return _instance;
        }

        /**
         * @brief Configure the ADC Controller
         *
         * @param config The config struct to use
         */
        void configure(const controller_config_t& config);

        /**
         * @brief Safely shuts down the controller and hardware
         */
        void shutdown();

        /**
         * @brief Adds an ADC input to the controller
         *
         * @param adc_channel The ADC channel to add
         */
        void add(adc_channel_t adc_channel);

        /**
         * @brief Removes an ADC input from the controller
         *
         * @param adc_channel The ADC channel to remove
         */
        void remove(adc_channel_t adc_channel);

        /**
         * @brief Checks if a specific ADC channel is registered in the controller
         *
         * @return Whether the pin is registered (true) or not (false)
         *
         * @param adc_channel The ADC channel to check for
         */
        [[nodiscard]] bool is_registered(adc_channel_t adc_channel) const;

        /**
         * @brief Reads the current raw ADC reading
         *
         * @return The raw ADC reading
         *
         * @param adc_channel The ADC channel to read from
         * @param samples The amount of samples to take when reading (default: 1)
         */
        [[nodiscard]] uint16_t read_raw(adc_channel_t adc_channel, uint16_t samples = 1) const;

        /**
         * @brief Reads the current voltage in millivolts
         *
         * @return The voltage in millivolts
         * 
         * @param adc_channel The ADC channel to read from
         * @param samples The amount of samples to take when reading (default: 1)
         */
        [[nodiscard]] uint16_t read_millivolts(adc_channel_t adc_channel, uint16_t samples = 1) const;
    };
}