// COLORSENSOR_CPP
// Boboter
// (C) MarioS271 2025

#include "colorsensor.h"

#include "logger.h"

#define TAG "COLORSENSOR"

Colorsensor::Colorsensor() {
    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = (1ULL << LED_PIN);
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_conf);

    LOGI(TAG, "Initialized Color Sensor");
}

void Colorsensor::setLedState(bool state) {
    gpio_set_level(LED_PIN, state);
    LOGI(TAG, "Set Color Sensor LED to %s", state ? "true" : "false");
}