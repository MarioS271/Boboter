/**
 * @file bumper.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "bumper.h"

#include "devices/ultrasonic/ultrasonic.h"
#include "include/robot.h"

namespace Device {
    Bumper::Bumper(Robot& robot) :
        robot(robot)
    {
        LOGI("Constructor called");
    }

    Bumper::~Bumper() {
        LOGI("Destructor called");
    }

    void Bumper::initialize() {
        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = LEFT_PIN,
                .mode = GPIO_MODE_INPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );

        robot.gpio.add(
            HAL::GPIO::pin_config_t{
                .gpio_pin = RIGHT_PIN,
                .mode = GPIO_MODE_INPUT,
                .pull_mode = GPIO_FLOATING,
                .intr_type = GPIO_INTR_DISABLE
            }
        );
    }

    bool Bumper::is_hit(const bumper_id_t bumper_id) const {
        using enum bumper_id_t;

        gpio_num_t pin = GPIO_NUM_NC;

        switch (bumper_id) {
            case LEFT: pin = LEFT_PIN; break;
            case RIGHT: pin = RIGHT_PIN; break;
            default: break;
        }

        return static_cast<bool>(robot.gpio.get_level(pin));
    }
}
