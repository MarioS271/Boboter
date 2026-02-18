/**
 * @file encoders.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "encoders.h"

#include "lib/logger/logger.h"
#include "lib/error/error.h"
#include "include/robot.h"

namespace Device {
    Encoders::Encoders(Robot& robot) :
        robot(robot),
        isr_contexts{}
    {
        LOGD("Constructor called");
    }

    Encoders::~Encoders() {
        LOGD("Destructor called");
    }

    void Encoders::encoder_isr_handler(void* arg) {
        const auto isr_context = static_cast<isr_context_t*>(arg);
        ++isr_context->instance->pulse_count[isr_context->encoder_index];

        LOGV("Detected an encoder pulse on the %s encoder", isr_context->encoder_index == 0 ? "left" : "right");
    }

    void Encoders::initialize() {
        constexpr gpio_num_t pins_to_initialize[NUM_ENCODERS] = { LEFT_ENCODER_PIN, RIGHT_ENCODER_PIN };
        for (uint8_t i = 0; i < NUM_ENCODERS; ++i) {
            robot.gpio.add(
                HAL::GPIO::pin_config_t{
                    .gpio_pin = pins_to_initialize[i],
                    .mode = GPIO_MODE_INPUT,
                    .pull_mode = GPIO_FLOATING,
                    .intr_type = GPIO_INTR_POSEDGE
                }
            );

            isr_contexts[i] = {
                .instance = this,
                .encoder_index = i
            };

            WARN_CHECK(gpio_isr_handler_add(pins_to_initialize[i], encoder_isr_handler, &isr_contexts[i]));
        }

        LOGI("Initialized Device::Encoders");
    }
}
