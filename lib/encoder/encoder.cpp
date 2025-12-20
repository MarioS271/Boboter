/**
 * @file encoder.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "encoder.hpp"

#include <freertos/FreeRTOS.h>
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Encoder {
    namespace Internal {
        void IRAM_ATTR encoder_isr_handler(void* arg) {
            Encoder* enc = reinterpret_cast<Encoder*>(arg);
            enc->pulse_count += 1;
        }
    }

    Encoder::Encoder(Boboter::Types::Encoder::Id encoder_id)
    : encoder_id(encoder_id),
      pulse_count(0)
    {
        using namespace Config;
        using namespace Boboter::Types::Encoder;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        switch (encoder_id) {
            case ENCODER_LEFT:
                LOGI(TAG, "Initialized ENCODER_LEFT (ID: %d)", encoder_id);
                encoder_pin = ENCODER_LEFT_PIN;
                break;
    
            case ENCODER_RIGHT:
                LOGI(TAG, "Initialized ENCODER_RIGHT (ID: %d)", encoder_id);
                encoder_pin = ENCODER_RIGHT_PIN;
                break;
    
            default:
                LOGE(TAG, "Unable to initialize Encoder (ID: %d)", encoder_id);
                return;
        }
    
        ERROR_CHECK(TAG, gpio_reset_pin(encoder_pin));
        ERROR_CHECK(TAG, gpio_config(&(gpio_config_t){
            .pin_bit_mask = (1ull << encoder_id),
            .mode = GPIO_MODE_INPUT,
            .intr_type = GPIO_INTR_POSEDGE,
        }));
    
        if (!isr_service_installed) {
            ERROR_CHECK(TAG, gpio_install_isr_service(0));
            isr_service_installed = true;
        }
        ERROR_CHECK(TAG, gpio_isr_handler_add(encoder_pin, Internal::encoder_isr_handler, static_cast<void*>(this)));
    }
}