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
        
        adc_oneshot_unit_handle_t adc_unit;
        adc_cali_handle_t cal_handle;

        uint16_t voltage_mv;
        uint8_t percentage;

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
        uint8_t get_percentage() const { return percentage; }

        /**
         * @brief Returns the current battery voltage in millivolts
         *
         * @return (uint16_t) The voltage
        */
        uint16_t get_voltage_mv() const { return voltage_mv; }
    };
}