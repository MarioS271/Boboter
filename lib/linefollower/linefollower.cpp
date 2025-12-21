/**
 * @file linefollower.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "linefollower.hpp"

#include "helpers/adc_utils.hpp"
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Linefollower {
    Linefollower::Linefollower() {
        using namespace Config;
        namespace ADC_Config = Boboter::Helpers::ADC::Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        ERROR_CHECK(TAG, gpio_reset_pin(LF_RIGHT_LEFT_PIN));
        ERROR_CHECK(TAG, gpio_config(&(gpio_config_t){
            .pin_bit_mask = (1ull << LF_RIGHT_LEFT_PIN),
            .mode = GPIO_MODE_OUTPUT
        }));
    
        adc_unit = ADC_Config::adc_handle;
        cal_handle = ADC_Config::cal_handle;
    
        adc_oneshot_chan_cfg_t chan_cfg = {
            .atten = ADC_Config::ADC_ATTEN,
            .bitwidth = ADC_Config::ADC_BITWIDTH
        };
        ERROR_CHECK(TAG, adc_oneshot_config_channel(adc_unit, ADC_CHANNEL, &chan_cfg));
    
        LOGI(TAG, "Initialized Linefollower");
    }
    
    Boboter::Types::Linefollower::Result Linefollower::get(Boboter::Types::Linefollower::Id module_id) {
        using namespace Config;
        using namespace Boboter::Libs::Error;

        WARN_CHECK(TAG, gpio_set_level(LF_RIGHT_LEFT_PIN, static_cast<uint32_t>(module_id)));
    
        int reading;
        ERROR_CHECK(TAG, adc_oneshot_read(adc_unit, ADC_CHANNEL, &reading));
    
        Boboter::Types::Linefollower::Result result;
        if (reading < 2300)
            result = Boboter::Types::Linefollower::Result::WHITE;
        else
            result = Boboter::Types::Linefollower::Result::BLACK;
    
        return result;
    }
}