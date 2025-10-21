// MAIN_CPP
// Boboter
// (C) MarioS271 2025

// Official Libraries
#include "esp_log.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"

// Custom Libraries
#include "leds.h"
#include "motors.h"
#include "encoder.h"
#include "bumper.h"
#include "ultrasonic.h"
#include "move.h"

// Headers
#include "flags.h"
#include "system_context.h"

// Routines
#include "sensor_test.h"
#include "led_task.h"

static const char* TAG = "MAIN";

extern "C" void app_main() {
    esp_log_level_set("*", SHOW_DEBUG_LOGS ? ESP_LOG_DEBUG : ESP_LOG_INFO);

    ESP_LOGI(TAG, " === BOBOTER is starting ===");
    ESP_LOGI(TAG, "Hello, World!");

    static Leds leds = Leds();
    static Motor motorL = Motor(MOTOR_LEFT);
    static Motor motorR = Motor(MOTOR_RIGHT);
    static Encoder encoderL = Encoder(ENCODER_LEFT);
    static Encoder encoderR = Encoder(ENCODER_RIGHT);
    static Bumper bumperL = Bumper(BUMPER_LEFT);
    static Bumper bumperR = Bumper(BUMPER_RIGHT);
    static Ultrasonic usonic = Ultrasonic();
    static Move move = Move(motorL, motorR, encoderL, encoderR, bumperL, bumperR, usonic);

    ESP_LOGI(TAG, "Created all Objects successfully");

    static SystemContext sysctx = {
        leds, motorL, motorR,
        encoderL, encoderR, bumperL,
        bumperR, usonic, move
    };

    ESP_LOGI(TAG, "Created SystemContext");
    ESP_LOGI(TAG, "Starting FreeRTOS Task(s)");

    if (ENABLE_SENSOR_TEST_MODE) { xTaskCreate(sensorTest, "SensorTest", 4096, &sysctx, 5, nullptr); return; }

    //// WEBSERVER TEST
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
}