/**
 * @file button.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "button.hpp"

#include <esp_timer.h>
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Button {
    Button::Button(Boboter::Types::Button::Id button_id)
    : button_id(button_id),
      last_press(0)
    {
        using namespace Config;
        using namespace Boboter::Types::Button;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        switch (button_id) {
            case Id::PRIMARY:
                LOGI(TAG, "Initialized BUTTON_PRIMARY (ID: %d)", button_id);
                button_pin = PRIMARY_PIN;
                break;

            case Id::SECONDARY:
                LOGI(TAG, "Initialized BUTTON_SECONDARY (ID: %d)", button_id);
                button_pin = SECONDARY_PIN;
                break;

            default:
                LOGE(TAG, "Failed to initialize button (ID: %d)", button_id);
                abort();
        }

        ERROR_CHECK(TAG, gpio_reset_pin(button_pin));
        ERROR_CHECK(TAG, gpio_config(&(gpio_config_t){
            .pin_bit_mask = (1ull << button_pin),
            .mode = GPIO_MODE_INPUT
        }));
    }

    bool Button::get_debounced_press() {
        using namespace Config;
        
        uint64_t current_time = esp_timer_get_time();

        if (current_time - last_press < DEBOUNCE_US && gpio_get_level(button_pin)) {
            last_press = current_time;
            return true;
        }

        return false;
    }
}