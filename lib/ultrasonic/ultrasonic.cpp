// ULTRASONIC_CPP
// Boboter
// (C) MarioS271 2025

#include "ultrasonic.h"

#include "esp_log.h"
#include "esp_timer.h"
#include <rom/ets_sys.h>
#include "freertos/FreeRTOS.h"
#include "delay.h"

#define TAG "ULTRASONIC"

Ultrasonic::Ultrasonic() {
    gpio_config_t gpio_conf = {};
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pin_bit_mask = (1ULL << TRIGGER_PIN);
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&gpio_conf);

    gpio_conf.mode = GPIO_MODE_INPUT;
    gpio_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    gpio_config(&gpio_conf);

    ESP_LOGI(TAG, "Ultrasonic initialized (Trigger=%d, Echo=%d)", TRIGGER_PIN, ECHO_PIN);
}

float Ultrasonic::measureCm() {
    // Send trigger pulse
    gpio_set_level(TRIGGER_PIN, 0);
    delay(2);
    gpio_set_level(TRIGGER_PIN, 1);
    ets_delay_us(10);
    gpio_set_level(TRIGGER_PIN, 0);

    // Wait for echo start
    int64_t start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 0) {
        if (esp_timer_get_time() - start > 30000) return -1;  // Timeout
    }

    // Measure echo high time
    int64_t echo_start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 1) {
        if (esp_timer_get_time() - echo_start > 30000) return -1;  // Timeout
    }
    int64_t echo_end = esp_timer_get_time();

    float duration_us = static_cast<float>(echo_end - echo_start);
    float distance_cm = duration_us / 58.0f; // Speed of sound: 343 m/s

    ESP_LOGD(TAG, "Measured Distance: %.2f cm", distance_cm);
    return distance_cm;
}
