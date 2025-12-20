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

namespace Boboter::Libs::RgbLeds {
    RgbLeds::RgbLeds() {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;
    
        ERROR_CHECK(TAG, gpio_set_direction(MOSI_PIN, GPIO_MODE_OUTPUT));
        ERROR_CHECK(TAG, gpio_set_direction(CLK_PIN, GPIO_MODE_OUTPUT));
    
        allOff();
    
        LOGI(TAG, "Initialized RGB LEDs");
    }
    
    void RgbLeds::setColor(Boboter::Types::RgbLeds::Id led_id, Boboter::Types::rgb_color_t color) {
        using namespace Boboter::Libs::Logger;

        if (led_id >= 4) {
            LOGW(TAG, "Invalid LED position: %d", led_id);
            return;
        }
    
        leds[led_id] = color;
        update();
    }
    
    void RgbLeds::setOff(Boboter::Types::RgbLeds::Id led_id) {
        using namespace Boboter::Libs::Logger;

        if (led_id >= 4) {
            LOGW(TAG, "Invalid LED position: %d", led_id);
            return;
        }
    
        setColor(led_id, Boboter::Helpers::Colors::OFF);
    }
    
    void RgbLeds::setAll(Boboter::Types::rgb_color_t color) {
        for (int i = 0; i < 4; i++)
            leds[i] = color;
        update();
    }
    
    void RgbLeds::allOff() {
        setAll(Boboter::Helpers::Colors::OFF);
    }
    
    Boboter::Types::rgb_color_t RgbLeds::getColor(Boboter::Types::RgbLeds::Id led_id) {
        using namespace Boboter::Libs::Logger;

        if (led_id >= 4) {
            LOGW(TAG, "Invalid LED position: %d", led_id);
            return Boboter::Helpers::Colors::OFF;
        }
        return leds[led_id];
    }
}