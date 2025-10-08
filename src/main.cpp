// MAIN_CPP
// Boboter
// (C) MarioS271 2025

#include "esp_log.h"
#include "esp_random.h"

#include "delay.h"
#include "hex_color.h"
#include "predef_colors.h"

#include "leds.h"
#include "motors.h"

using namespace COLORS;

static const char* TAG = "app_main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Hello, World!");

    Leds leds = Leds();
    leds.allOff();
    
    hex_color_t colorArray[] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
    constexpr int numColors = sizeof(colorArray) / sizeof(colorArray[0]);

    hex_color_t colors[4] = {};

    while (true) {
        for (int i = 0; i < 4; i++) {
            colors[i] = colorArray[esp_random() % numColors];
        }

        leds.setColor(LED_FRONT_LEFT, RED);
        leds.setColor(LED_FRONT_RIGHT, ORANGE);
        leds.setColor(LED_BACK_LEFT, YELLOW);
        leds.setColor(LED_BACK_RIGHT, GREEN);
        delay(1000);
    }
}