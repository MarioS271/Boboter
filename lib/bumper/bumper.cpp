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
        using enum Boboter::Types::Bumper::Id;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        switch (bumper_id) {
            case BUMPER_LEFT:
                LOGI(TAG, "Initialized LEFT_BUMPER (ID: %d)", bumper_id);
                bumper_pin = LEFT_BUMPER_PIN;
                break;
    
            case BUMPER_RIGHT:
                LOGI(TAG, "Initialized RIGHT_BUMPER (ID: %d)", bumper_id);
                bumper_pin = RIGHT_BUMPER_PIN;
                break;
    
            default:
                LOGE(TAG, "Unable to initialize Bumper (ID: %d)", bumper_id);
                return;
        }
    
        ERROR_CHECK(TAG, gpio_reset_pin(bumper_pin));
        ERROR_CHECK(TAG, gpio_config(&(gpio_config_t){
            .pin_bit_mask = (1ull << bumper_pin),
            .mode = GPIO_MODE_INPUT,
        }));
    }
}