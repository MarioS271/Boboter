/**
 * @file bumper.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "bumper.hpp"

#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Bumper {
    Bumper::Bumper(Boboter::Types::Bumper::Id bumper_id)
    : bumper_id(bumper_id)
    {
        using namespace Config;
        using namespace Boboter::Types::Bumper;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        switch (bumper_id) {
            case Id::LEFT:
                LOGI(TAG, "Initialized LEFT_BUMPER (ID: %d)", bumper_id);
                bumper_pin = LEFT_BUMPER_PIN;
                break;
    
            case Id::RIGHT:
                LOGI(TAG, "Initialized RIGHT_BUMPER (ID: %d)", bumper_id);
                bumper_pin = RIGHT_BUMPER_PIN;
                break;
    
            default:
                LOGE(TAG, "Unable to initialize Bumper (ID: %d)", bumper_id);
                abort();
        }
    
        gpio_config_t config = {
            .pin_bit_mask = (1ull << bumper_pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
        ERROR_CHECK(TAG, gpio_reset_pin(bumper_pin));
        ERROR_CHECK(TAG, gpio_config(&config));
    }
}