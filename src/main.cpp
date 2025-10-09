// MAIN_CPP
// Boboter
// (C) MarioS271 2025

#include "esp_log.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"

#include "flags.h"
#include "main_function_defs.h"
#include "delay.h"
#include "rgb_color.h"
#include "predef_colors.h"

#include "leds.h"
#include "motors.h"
#include "ultrasonic.h"

using namespace COLORS;

static const char* TAG = "MAIN";

extern "C" void app_main() {
    esp_log_level_set("*", SHOW_DEBUG_LOGS ? ESP_LOG_DEBUG : ESP_LOG_INFO);
    ESP_LOGI(TAG, "Hello, World!");

    Leds leds = Leds();
    Motor motorLeft = Motor(MOTOR_LEFT);
    Motor motorRight = Motor(MOTOR_RIGHT);
    Ultrasonic usonic = Ultrasonic();

    leds.setStatusLed(true);
    xTaskCreate(ledTask, "LedTask", 2048, &leds, 5, nullptr);
    
    // motorLeft.setDirection(FORWARD);
    // motorRight.setDirection(BACKWARD);
    // motorLeft.setSpeed(500);
    // motorRight.setSpeed(500);

    while (true) {
        delay(5000);
    }
}



void ledTask(void* pvParameters) {
    Leds* leds = static_cast<Leds*>(pvParameters);

    rgb_color_t colorArray[] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
    constexpr int numColors = sizeof(colorArray) / sizeof(colorArray[0]);
    rgb_color_t colors[4] = {};

    while (true) {
        for (int i = 0; i < 4; i++) {
            colors[i] = colorArray[esp_random() % numColors];
        }

        leds->setColor(LED_FRONT_LEFT, colors[0]);
        leds->setColor(LED_FRONT_RIGHT, colors[1]);
        leds->setColor(LED_BACK_LEFT, colors[2]);
        leds->setColor(LED_BACK_RIGHT, colors[3]);

        delay(1000);
    }
}