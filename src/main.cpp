// MAIN_CPP
// Boboter
// (C) MarioS271 2025

// Libraries
#include "esp_log.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"

// Headers
#include "flags.h"

// Routines
#include "sensor_test.h"
#include "led_task.h"

using namespace COLORS;

static const char* TAG = "MAIN";

extern "C" void app_main() {
    esp_log_level_set("*", SHOW_DEBUG_LOGS ? ESP_LOG_DEBUG : ESP_LOG_INFO);

    ESP_LOGI(TAG, " === BOBOTER is starting ===");
    ESP_LOGI(TAG, "Hello, World!");

    if (ENABLE_SENSOR_TEST_MODE) { xTaskCreate(sensorTest, "SensorTest", 4096, nullptr, 5, nullptr); return; }

    //// WEBSERVER TEST
    // WebUI webui = WebUI();
    // Leds leds = Leds();

    // static rgb_color_t ledUL, ledUR, ledLL, ledLR;
    // WebUI webui = WebUI();

    // webui.led_upper_left = &ledUL;
    // webui.led_upper_right = &ledUR;
    // webui.led_lower_left = &ledLL;
    // webui.led_lower_right = &ledLR;

    // delay(500);
    // webui.startServer();

    // rgb_color_t* ledArray[4] = { &ledUL, &ledUR, &ledLL, &ledLR };
    // while (true) {
    //     for (uint8_t i = 0; i < 4; i++) {
    //         leds.setColor(static_cast<led_pos_t>(i), *ledArray[i]);
    //     }
        
    //     delay(250);
    // }
    ////////

    //// MOVE ENGINE TEST
    // Move move = Move();

    // move.forward(20, MAX_MOTOR_SPEED);
    // delay(1000);
    // move.turn(180, 800);
    // delay(1000);
    // move.backward(20, MAX_MOTOR_SPEED);
    ////////
}