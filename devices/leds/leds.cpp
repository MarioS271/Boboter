/**
 * @file leds.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "leds.h"

#include "include/robot.h"
#include "helpers/predef_colors.h"
#include "lib/logger/logger.h"

namespace Device {
    Leds::Leds(Robot& robot) :
        robot(robot),
        leds()
    {
        LOGI("Constructor called");
    }

    Leds::~Leds() {
        LOGI("Destructor called");
        shutdown();
    }

    void Leds::initialize() {
        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = MOSI_PIN,
                .mode = GPIO_MODE_OUTPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = SCK_PIN,
                .mode = GPIO_MODE_OUTPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        turn_all_off();

        LOGI("Initialized Device::Leds");
    }

    void Leds::shutdown() {
        turn_all_off();
    }

    void Leds::update() const {
        for (int i = 0; i < 4; ++i) send_byte(0x00);
        for (int i = 0; i < NUM_LEDS; ++i) send_frame(leds[i]);
        for (int i = 0; i < 4; ++i) send_byte(0xFF);
    }

    void Leds::set_color(const led_id_t led_id, const rgb_color_t color, const bool do_update) {
        leds[static_cast<uint8_t>(led_id)] = color;
        if (do_update) {
            update();
        }
    }

    void Leds::set_color_all(const rgb_color_t color) {
        for (auto& led : leds) {
            led = color;
        }

        update();
    }

    void Leds::turn_off(const led_id_t led_id, const bool do_update) {
        leds[static_cast<uint8_t>(led_id)] = Colors::OFF;
        if (do_update) {
            update();
        }
    }

    void Leds::turn_all_off() {
        for (auto& led : leds) {
            led = Colors::OFF;
        }

        update();
    }
}
