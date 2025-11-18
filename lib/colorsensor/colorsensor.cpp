// COLORSENSOR_CPP
// Boboter
// (C) MarioS271 2025

#include "colorsensor.h"
#include "logger.h"

Colorsensor::Colorsensor() {
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    LOGI(TAG, "Initialized Color Sensor");
}

void Colorsensor::setLedState(bool state) {
    gpio_set_level(LED_PIN, state);
    LOGI(TAG, "Set Color Sensor LED to %s", state ? "true" : "false");
}