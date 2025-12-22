/**
 * @file leds.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "rgb_leds.hpp"

#include "helpers/predef_colors.hpp"
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::RGB_Leds {
    RGB_Leds::RGB_Leds() {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;
    
        gpio_config_t config = {
            .pin_bit_mask = (1ull << MOSI_PIN) | (1ull << CLK_PIN),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
        ERROR_CHECK(TAG, gpio_reset_pin(MOSI_PIN));
        ERROR_CHECK(TAG, gpio_reset_pin(CLK_PIN));
        ERROR_CHECK(TAG, gpio_config(&config));
    
        all_off();
    
        LOGI(TAG, "Initialized RGB LEDs");
    }
    
    void RGB_Leds::set_color(Boboter::Types::RGB_Leds::Id led_id, Boboter::Types::rgb_color_t color) {
        using namespace Boboter::Libs::Logger;

        if (led_id >= 4) {
            LOGW(TAG, "Invalid LED position: %d", led_id);
            return;
        }
    
        leds[led_id] = color;
        update();
    }
    
    void RGB_Leds::set_off(Boboter::Types::RGB_Leds::Id led_id) {
        using namespace Boboter::Libs::Logger;

        if (led_id >= 4) {
            LOGW(TAG, "Invalid LED position: %d", led_id);
            return;
        }
    
        set_color(led_id, Boboter::Helpers::Colors::OFF);
    }
    
    void RGB_Leds::set_all(Boboter::Types::rgb_color_t color) {
        for (int i = 0; i < 4; i++)
            leds[i] = color;
        update();
    }
    
    void RGB_Leds::all_off() {
        set_all(Boboter::Helpers::Colors::OFF);
    }
    
    Boboter::Types::rgb_color_t RGB_Leds::get_color(Boboter::Types::RGB_Leds::Id led_id) {
        using namespace Boboter::Libs::Logger;

        if (led_id >= 4) {
            LOGW(TAG, "Invalid LED position: %d", led_id);
            return Boboter::Helpers::Colors::OFF;
        }
        return leds[led_id];
    }
}