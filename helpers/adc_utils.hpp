/**
 * @file adc_utils.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include "lib/error/error.hpp"

namespace Boboter::Helpers::ADC {
    namespace Config {
        constexpr adc_unit_t ADC_UNIT = ADC_UNIT_1;
        constexpr adc_atten_t ADC_ATTEN = ADC_ATTEN_DB_12;
        constexpr adc_bitwidth_t ADC_BITWIDTH = ADC_BITWIDTH_DEFAULT;
        constexpr adc_ulp_mode_t ADC_ULP_MODE = ADC_ULP_MODE_DISABLE;
        
        inline adc_oneshot_unit_handle_t adc_handle = nullptr;
        inline adc_cali_handle_t cal_handle = nullptr;
    }
    
    namespace Internal {
        constexpr const char* TAG = "Helpers::ADC";
    }

    /**
     * @brief Initializes and configures ADC
     *
     * @return (void)  
    */
    inline void init() {
        using namespace Internal;
        using namespace Config;
        using namespace Boboter::Libs::Error;

        if (adc_handle != nullptr)
            return;

        adc_oneshot_unit_init_cfg_t unit_cfg = {
            .unit_id = ADC_UNIT,
            .ulp_mode = ADC_ULP_MODE
        };
        ERROR_CHECK(TAG, adc_oneshot_new_unit(&unit_cfg, &adc_handle));

        adc_cali_line_fitting_config_t cal_cfg = {
            .unit_id = ADC_UNIT,
            .atten = ADC_ATTEN,
            .bitwidth = ADC_BITWIDTH
        };
        if (adc_cali_create_scheme_line_fitting(&cal_cfg, &cal_handle) != ESP_OK)
            cal_handle = nullptr;
    }
}