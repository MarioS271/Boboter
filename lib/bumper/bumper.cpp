// BUMPER_CPP
// Boboter
// (C) MarioS271 2025

#include "bumper.h"
#include "esp_log.h"

static const char* TAG = "BUMPER";

// Constructor
Bumper::Bumper(bumper_num_t bumper_number)
: bumper_num(bumper_number),
  error(false)
{
    switch (bumper_num) {
        case BUMPER_LEFT:
            ESP_LOGI(TAG, "Initialized Bumper LEFT_BUMPER (ID: %d)", bumper_num);
            bumper_pin = LEFT_BUMPER_PIN;
            break;

        case BUMPER_RIGHT:
            ESP_LOGI(TAG, "Initialized Bumper RIGHT_BUMPER (ID: %d)", bumper_num);
            bumper_pin = RIGHT_BUMPER_PIN;
            break;

        default:
            ESP_LOGE(TAG, "Unable to initialize Bumper (ID: %d)", bumper_num);
            error = true;
            return;
    }

    gpio_config_t gpio_conf = {};
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_conf.mode = GPIO_MODE_INPUT;
    gpio_conf.pin_bit_mask = (1ULL << bumper_pin);
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&gpio_conf);
}



// Check if Bumper is pressed
bool Bumper::isHit() const {
    bool state = !gpio_get_level(bumper_pin);
    if (state) ESP_LOGD(TAG, "Bumper hit while calling isHit() of %s", bumper_num == BUMPER_LEFT ? "BUMPER_LEFT" : "BUMPER_RIGHT");
    return state;
}