// ENCODER_CPP
// Boboter
// (C) MarioS271 2025

#include "encoder.h"

#include <freertos/FreeRTOS.h>
#include "logger.h"
#include "error.h"

void IRAM_ATTR encoder_isr_handler(void* arg) {
    Encoder* enc = reinterpret_cast<Encoder*>(arg);
    enc->pulse_count += 1;
}

Encoder::Encoder(encoder_num_t encoder_number)
: encoder_num(encoder_number),
  error(false),
  pulse_count(0)
{
    switch (encoder_num) {
        case ENCODER_LEFT:
            LOGI(TAG, "Initialized Encoder ENCODER_LEFT (ID: %d)", encoder_num);
            encoder_pin = ENCODER_LEFT_PIN;
            break;

        case ENCODER_RIGHT:
            LOGI(TAG, "Initialized Encoder ENCODER_RIGHT (ID: %d)", encoder_num);
            encoder_pin = ENCODER_RIGHT_PIN;
            break;

        default:
            LOGE(TAG, "Unable to initialize Encoder (ID: %d)", encoder_num);
            error = true;
            return;
    }

    ERROR_CHECK(TAG, gpio_reset_pin(encoder_pin));
    ERROR_CHECK(TAG, gpio_set_direction(encoder_pin, GPIO_MODE_INPUT));
    ERROR_CHECK(TAG, gpio_set_intr_type(encoder_pin, GPIO_INTR_POSEDGE));

    static bool isr_service_installed = false;
    if (!isr_service_installed) {
        ERROR_CHECK(TAG, gpio_install_isr_service(0));
        isr_service_installed = true;
    }
    ERROR_CHECK(TAG, gpio_isr_handler_add(encoder_pin, encoder_isr_handler, (void*) this));
}