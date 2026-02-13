/**
 * @file buttons.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "buttons.h"

#include "include/robot.h"
#include "lib/logger/logger.h"

namespace Device {
    Buttons::Buttons(Robot& robot) :
        robot(robot),
        primary_button_state(false),
        secondary_button_state(false)
    {
        LOGD("Constructor called");
    }

    Buttons::~Buttons() {
        LOGD("Destructor called");
    }

    void Buttons::initialize() {
        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = BUTTON_PRIMARY_PIN,
                .mode = GPIO_MODE_INPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = BUTTON_SECONDARY_PIN,
                .mode = GPIO_MODE_INPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        LOGI("Initialized Device::Buttons");
    }

    void Buttons::update_button_states() {
        primary_button_state = static_cast<bool>(robot.gpio.get_level(BUTTON_PRIMARY_PIN));
        secondary_button_state = static_cast<bool>(robot.gpio.get_level(BUTTON_SECONDARY_PIN));
    }
}
