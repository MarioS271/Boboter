// ULTRASONIC_CPP
// Boboter
// (C) MarioS271 2025

#include "ultrasonic.h"

#include "delay.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "rom/ets_sys.h"
#include "esp_timer.h"

Ultrasonic::Ultrasonic() {
    gpio_set_direction(TRIGGER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);

    LOGI(TAG, "Ultrasonic initialized");
}

float Ultrasonic::measureCm() {
    gpio_set_level(TRIGGER_PIN, 0);
    delay(2);
    gpio_set_level(TRIGGER_PIN, 1);
    ets_delay_us(10);
    gpio_set_level(TRIGGER_PIN, 0);

    int64_t start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 0) {
        if (esp_timer_get_time() - start > 30000) return -1;
    }

    int64_t echo_start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 1) {
        if (esp_timer_get_time() - echo_start > 30000) return -1;
    }
    int64_t echo_end = esp_timer_get_time();

    float duration_us = static_cast<float>(echo_end - echo_start);
    float distance_cm = duration_us / 58.0f;  // 343 m/s

    return distance_cm;
}