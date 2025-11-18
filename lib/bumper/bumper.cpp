// BUMPER_CPP
// Boboter
// (C) MarioS271 2025

#include "bumper.h"
#include "logger.h"

Bumper::Bumper(bumper_num_t bumper_number)
: bumper_num(bumper_number),
  error(false)
{
    switch (bumper_num) {
        case BUMPER_LEFT:
            LOGI(TAG, "Initialized Bumper LEFT_BUMPER (ID: %d)", bumper_num);
            bumper_pin = LEFT_BUMPER_PIN;
            break;

        case BUMPER_RIGHT:
            LOGI(TAG, "Initialized Bumper RIGHT_BUMPER (ID: %d)", bumper_num);
            bumper_pin = RIGHT_BUMPER_PIN;
            break;

        default:
            LOGE(TAG, "Unable to initialize Bumper (ID: %d)", bumper_num);
            error = true;
            return;
    }

    gpio_set_direction(bumper_pin, GPIO_MODE_INPUT);
}

bool Bumper::isHit() const {
    bool state = !gpio_get_level(bumper_pin);
    if (state) LOGI(TAG, "Bumper hit while calling isHit() of %s", bumper_num == BUMPER_LEFT ? "BUMPER_LEFT" : "BUMPER_RIGHT");
    return state;
}