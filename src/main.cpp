// MAIN_CPP
// Boboter
// (C) MarioS271 2025

#include "esp_log.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "delay.h"
#include "hex_color.h"
#include "predef_colors.h"

#include "leds.h"
#include "motors.h"

using namespace COLORS;

static const char* TAG = "app_main";

void ledTask(void* pvParameters);

extern "C" void app_main() {
    ESP_LOGI(TAG, "Hello, World!");

    Leds leds = Leds();
    Motor motorLeft = Motor(MOTOR_LEFT);
    Motor motorRight = Motor(MOTOR_RIGHT);

    xTaskCreate(
        ledTask,         // Task function
        "LED Task",      // Task name
        2048,            // Stack size in words
        &leds,           // Parameter (pointer to Leds)
        5,               // Task priority
        nullptr          // Task handle (not needed here)
    );

    ESP_LOGI(TAG, "Motortest: vorwärts 2 Sekunden...");

    motorLeft.setDirection(FORWARD);
    motorRight.setDirection(FORWARD);
    motorLeft.setSpeed(1023);
    motorRight.setSpeed(1023);

    delay(2000);

    ESP_LOGI(TAG, "Stoppe Motoren...");
    motorLeft.stop();
    motorRight.stop();

    delay(1000);

    ESP_LOGI(TAG, "Motortest: rückwärts 2 Sekunden...");

    motorLeft.setDirection(BACKWARD);
    motorRight.setDirection(BACKWARD);
    motorLeft.setSpeed(800);
    motorRight.setSpeed(800);

    delay(2000);

    ESP_LOGI(TAG, "Motor-Test abgeschlossen.");
    motorLeft.stop();
    motorRight.stop();

    // Hauptloop
    while (true) {
        delay(5000);
    }
}


void ledTask(void* pvParameters) {
    Leds* leds = static_cast<Leds*>(pvParameters);

    hex_color_t colorArray[] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
    constexpr int numColors = sizeof(colorArray) / sizeof(colorArray[0]);
    hex_color_t colors[4] = {};

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