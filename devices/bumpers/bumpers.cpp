/**
 * @file bumpers.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "bumpers.h"

#include "include/robot/robot.h"

namespace Device {
    Bumpers::Bumpers(Robot& robot) :
        robot(robot)
    {
        LOGD("Constructor called");
    }

    Bumpers::~Bumpers() {
        LOGD("Destructor called");
    }

    void Bumpers::initialize() {
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
        
        LOGI("Initialized Device::Bumpers");
    }

    bool Bumpers::is_hit(const bumper_id_t bumper_id) const {
        using enum bumper_id_t;

        gpio_num_t pin = GPIO_NUM_NC;

        switch (bumper_id) {
            case LEFT: pin = LEFT_PIN; break;
            case RIGHT: pin = RIGHT_PIN; break;
            default: break;
        }

        const bool result = static_cast<bool>(robot.gpio.get_level(pin));

        LOGV("%s bumper is hit: %s", bumper_id == LEFT ? "LEFT" : "RIGHT", result ? "true" : "false");
        return result;
    }
}
