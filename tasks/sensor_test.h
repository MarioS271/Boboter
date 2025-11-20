// SENSOR_TEST_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "system_context.h"
#include "rgb_color.h"
#include "predef_colors.h"
#include "delay.h"
#include "logger.h"
#include "leds.h"
#include "motors.h"
#include "encoder.h"
#include "ultrasonic.h"
#include "bumper.h"

namespace SENSOR_TEST_TASK {
    constexpr const char* TAG = "task:SENSOR_TEST";
    constexpr float MAX_DISPLAY_DISTANCE = 100.0;
    constexpr float MIN_DISPLAY_DISTANCE = 5.0;
}

void sensorTest(void* params) {
    using namespace SENSOR_TEST_TASK;
    using namespace COLORS;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    Leds &leds = ctx->leds;
    Motor &motorL = ctx->motorL;
    Motor &motorR = ctx->motorR;
    Bumper &bumperL = ctx->bumperL;
    Bumper &bumperR = ctx->bumperR;
    Ultrasonic &usonic = ctx->usonic;

    LOGI(TAG, "Readied SystemContext and Object References");

    // 1) Iterate through all predefined colors to test the LEDs
    rgb_color_t colorArray[NUM_COLORS] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
    const char* colorNames[NUM_COLORS] = { "RED", "ORANGE", "YELLOW", "GREEN", "CYAN", "BLUE", "MAGENTA", "WHITE"};
    for (int i = 0; i < NUM_COLORS; i++) {
        leds.setAll(colorArray[i]);
        LOGI(TAG, "Set all LEDs to %s", colorNames[i]);
        delay(500);
    }
    delay(2000);

    // 2) Test Motors
    for (int i = 0; i < 2; i++) {
        motorL.setDirection(static_cast<motor_direction_t>(i));
        motorR.setDirection(static_cast<motor_direction_t>(i));

        motorL.setSpeed(MAX_MOTOR_SPEED);
        motorR.setSpeed(MAX_MOTOR_SPEED);

        LOGI(TAG, "Started both Motors %s with speed MAX_MOTOR_SPEED (%d)", i == 0 ? "FORWARD" : "BACKWARD", MAX_MOTOR_SPEED);
        delay(2000);

        motorL.stop(false);
        motorR.stop();

        LOGI(TAG, "Stopped both Motors");
        delay(1000);
    }
    delay(1000);

    // 3) Sensor Testing Mode
    leds.allOff();
    while (true) {
        if (bumperL.isHit()) {
            leds.setColor(LED_FRONT_LEFT, WHITE);
            LOGI(TAG, "Bumper_Left has hit something!");
        } else {
            leds.setOff(LED_FRONT_LEFT);
        }

        if (bumperR.isHit()) {
            leds.setColor(LED_FRONT_RIGHT, WHITE);
            LOGI(TAG, "Bumper_Right has hit something!");
        } else {
            leds.setOff(LED_FRONT_RIGHT);
        }

        float distance = usonic.measureCm();
        if (distance > MAX_DISPLAY_DISTANCE || distance <= 0) {
            leds.setColor(LED_BACK_LEFT, BLUE);
            leds.setColor(LED_BACK_RIGHT, BLUE);
        } else {
            LOGI(TAG, "Ultrasonic Sensor Reading: %.2f", distance);

            float t = (distance - MIN_DISPLAY_DISTANCE) / (MAX_DISPLAY_DISTANCE - MIN_DISPLAY_DISTANCE);
            t = t > 1.0 ? 1.0 : (t < 0 ? 0 : t);

            rgb_color_t color;
            if (t < 0.33) {
                float subT = t / 0.33;
                color.r = 255;
                color.g = (uint8_t)(255 * subT);
                color.b = 0;
            } else if (t < 0.66) {
                float subT = (t - 0.33) / 0.33;
                color.r = (uint8_t)(255 * (1.0 - subT));
                color.g = 255;
                color.b = 0;
            } else {
                float subT = (t - 0.66) / 0.34;
                color.r = 0;
                color.g = (uint8_t)(255 * (1.0 - subT));
                color.b = (uint8_t)(255 * subT);
            }

            leds.setColor(LED_BACK_LEFT, color);
            leds.setColor(LED_BACK_RIGHT, color);
        }
        
        delay(200);
    }
}