// ENCODER_CPP
// Boboter
// (C) MarioS271 2025

#include "encoder.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

static const char* TAG = "ENCODER";

void IRAM_ATTR encoder_isr_handler(void* arg) {
    Encoder* enc = reinterpret_cast<Encoder*>(arg);
    enc->pulse_count++;
}

// Constructor
Encoder::Encoder(encoder_num_t encoder_number)
: encoder_num(encoder_number),
  error(false),
  pulse_count(0) {
    switch (encoder_num) {
        case ENCODER_LEFT:
            ESP_LOGI(TAG, "Initialized Encoder ENCODER_LEFT (ID: %d)", encoder_num);
            encoder_pin = ENCODER_LEFT_PIN;
            break;

        case ENCODER_RIGHT:
            ESP_LOGI(TAG, "Initialized Encoder ENCODER_RIGHT (ID: %d)", encoder_num);
            encoder_pin = ENCODER_RIGHT_PIN;
            break;

        default:
            ESP_LOGI(TAG, "Unable to initialize Encoder (ID: %d)", encoder_num);
            error = true;
            break;
    }

    gpio_config_t gpio_conf = {};
    gpio_conf.intr_type = GPIO_INTR_POSEDGE;
    gpio_conf.mode = GPIO_MODE_INPUT;
    gpio_conf.pin_bit_mask = (1ULL << encoder_pin);
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&gpio_conf);

    static bool isr_service_installed = false;
    if (!isr_service_installed) {
        gpio_install_isr_service(0);
        isr_service_installed = true;
    }
    gpio_isr_handler_add(encoder_pin, encoder_isr_handler, (void*) this);
}



// Getter for Pulse Count
int32_t Encoder::getPulseCount() {
    return pulse_count;
}

// Resetter for Pulse Count
void Encoder::resetPulseCount() {
    pulse_count = 0;
}



// Get if Encoder has Error
bool Encoder::hasError() {
    return error;
}