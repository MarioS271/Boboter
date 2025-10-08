// MAIN_CPP
// Boboter
// (C) MarioS271 2025

#include "esp_log.h"

#include "delay.h"
#include "leds.h"
#include "motors.h"

static const char* TAG = "app_main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Hello, World!");

    Leds leds = Leds();
    
    leds.set_color();
}