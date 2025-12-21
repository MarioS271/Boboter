/**
 * @file linefollower.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "linefollower_types.hpp"

#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>

namespace Boboter::Libs::Linefollower {
    namespace Config {
        constexpr gpio_num_t LF_RIGHT_LEFT_PIN = GPIO_NUM_12;
        constexpr adc_channel_t ADC_CHANNEL = ADC_CHANNEL_0;
    }

    class Linefollower {
    private:
        static constexpr const char* TAG = "Libs::Linefollower";

        adc_oneshot_unit_handle_t adc_unit;
        adc_cali_handle_t cal_handle;

    public:
        explicit Linefollower();
        ~Linefollower() = default;

        /**
         * @brief Gets the current reading of one of the line follower modules
         *
         * @return (Boboter::Types::Linefollower::Result) The result of the reading (either BLACK or WHITE)
         *
         * @param module_id The linefollower module id
        */
        Boboter::Types::Linefollower::Result get(Boboter::Types::Linefollower::Id module_id);
    };
}