/**
 * @file ultrasonic.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "ultrasonic.hpp"

#include <freertos/FreeRTOS.h>
#include <rom/ets_sys.h>
#include <esp_timer.h>
#include "helpers/delay.hpp"
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Ultrasonic {
    Ultrasonic::Ultrasonic() {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;
    
        ERROR_CHECK(TAG, gpio_reset_pin(TRIGGER_PIN));
        ERROR_CHECK(TAG, gpio_set_direction(TRIGGER_PIN, GPIO_MODE_OUTPUT));
    
        ERROR_CHECK(TAG, gpio_reset_pin(ECHO_PIN));
        ERROR_CHECK(TAG, gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT));
    
        LOGI(TAG, "Initialized Ultrasonic");
    }
    
    float Ultrasonic::measureCm() {
        using namespace Config;
        
        gpio_set_level(TRIGGER_PIN, 0);
        ets_delay_us(2000);
        gpio_set_level(TRIGGER_PIN, 1);
        ets_delay_us(10);
        gpio_set_level(TRIGGER_PIN, 0);
    
        int64_t start = esp_timer_get_time();
        while (gpio_get_level(ECHO_PIN) == 0) {
            if (esp_timer_get_time() - start > 30000)
                return -1;
        }
    
        int64_t echo_start = esp_timer_get_time();
        while (gpio_get_level(ECHO_PIN) == 1) {
            if (esp_timer_get_time() - echo_start > 30000)
                return -1;
        }
        int64_t echo_end = esp_timer_get_time();
    
        float duration_us = static_cast<float>(echo_end - echo_start);
        float distance_cm = duration_us / 58.0f;  // 343 m/s
    
        return distance_cm;
    }
}