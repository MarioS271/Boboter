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
#include "move.h"
#include "webui.h"

using namespace COLORS;

static const char* TAG = "MAIN";

extern "C" void app_main() {
    esp_log_level_set("*", SHOW_DEBUG_LOGS ? ESP_LOG_DEBUG : ESP_LOG_INFO);

    ESP_LOGI(TAG, " === BOBOTER is starting ===");
    ESP_LOGI(TAG, "Hello, World!");

    Leds leds = Leds();
    Move move = Move();

    static rgb_color_t ledUL, ledUR, ledLL, ledLR;
    WebUI webui = WebUI();

    webui.led_upper_left = &ledUL;
    webui.led_upper_right = &ledUR;
    webui.led_lower_left = &ledLL;
    webui.led_lower_right = &ledLR;

    delay(500);
    // webui.startServer();

    leds.setStatusLed(true);
    // xTaskCreate(ledTask, "LedTask", 2048, &leds, 5, nullptr);

    // move.forward(20, MAX_SPEED_MOTOR);
    // delay(1000);
    // move.turn(180, 800);
    // delay(1000);
    // move.backward(20, MAX_SPEED_MOTOR);

    rgb_color_t* ledArray[4] = { &ledUL, &ledUR, &ledLL, &ledLR };
    while (true) {
        for (uint8_t i = 0; i < 4; i++) {
            leds.setColor(static_cast<led_pos_t>(i), *ledArray[i]);
        }
        
        delay(250);
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