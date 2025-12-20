/**
 * @file battery.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>
#include <driver/gpio.h>
#include "helpers/adc_utils.hpp"

namespace Boboter::Libs::Battery {
    namespace Config {
        constexpr uint8_t NUM_SAMPLES = 16;
        constexpr gpio_num_t ADC_GPIO = GPIO_NUM_39;
        constexpr adc_channel_t ADC_CHANNEL = ADC_CHANNEL_3;
    }

    class Battery {
    private:
        static constexpr const char* TAG = "Libs::Battery";
        
        adc_oneshot_unit_handle_t adc_unit = nullptr;
        adc_cali_handle_t cal_handle = nullptr;

        uint16_t voltage_mv = 0;
        uint8_t percentage = 0;

    public:
        explicit Battery();
        ~Battery() = default;

        /**
         * @brief Read and calculate current battery state
         *
         * @return (void)  
        */
        void update();

        /**
         * @brief Returns the current battery percentage in percent
         *
         * @return (uint8_t) The percentage
        */
        uint8_t getPercentage() const { return percentage; }

        /**
         * @brief Returns the current battery voltage in millivolts
         *
         * @return (uint16_t) The voltage
        */
        uint16_t getVoltageMv() const { return voltage_mv; }
    };
}