/**
 * @file other_leds.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "other_leds.hpp"

#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::OtherLeds {
    OtherLeds::OtherLeds()
    : status_led_state(false),
      bottom_led_state(false)
    {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;
        
        ERROR_CHECK(TAG, gpio_reset_pin(STATUS_LED_PIN));
        ERROR_CHECK(TAG, gpio_reset_pin(BOTTOM_LED_PIN));
        ERROR_CHECK(TAG, gpio_config(&(gpio_config_t){
            .pin_bit_mask = (1ull << STATUS_LED_PIN) | (1ull << BOTTOM_LED_PIN),
            .mode = GPIO_MODE_OUTPUT,
        }));

        LOGI(TAG, "Initialized Other Leds");
    }

    void OtherLeds::setStatusLed(bool new_state) {
        using namespace Config;
        using namespace Boboter::Libs::Error;

        status_led_state = new_state;
        WARN_CHECK(TAG, gpio_set_level(STATUS_LED_PIN, status_led_state));
    }

    void OtherLeds::setBottomLed(bool new_state) {
        using namespace Config;
        using namespace Boboter::Libs::Error;

        bottom_led_state = new_state;
        WARN_CHECK(TAG, gpio_set_level(BOTTOM_LED_PIN, bottom_led_state));
    }
}