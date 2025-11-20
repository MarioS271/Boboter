// LED_TASK_H
// Boboter
// (C) MarioS271, ThomS263 2025

#pragma once

#include <esp_random.h>
#include <esp_timer.h>
#include "delay.h"
#include "system_context.h"
#include "flex_struct.h"
#include "rgb_color.h"
#include "predef_colors.h"
#include "leds.h"

// void ledTask(void* params) {
//     using namespace COLORS;

//     SystemContext* ctx = static_cast<SystemContext*>(params);
//     FlexStruct &ownFlex = ctx->ledsFlex;
//     FlexStruct &ioShieldFlex = ctx->ioShieldFlex;
//     Leds &leds = ctx->leds;

//     static char mode_str[32] = "random_blink";
//     ownFlex.set<char*>("mode", mode_str);
    
//     ownFlex.set<rgb_color_t>("led_ur", OFF);
//     ownFlex.set<rgb_color_t>("led_ul", OFF);
//     ownFlex.set<rgb_color_t>("led_lr", OFF);
//     ownFlex.set<rgb_color_t>("led_ll", OFF);

//     rgb_color_t colorArray[8] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
//     rgb_color_t colors[4] = {};

//     while (true) {
//         for (int i = 0; i < 4; i++) {
//             colors[i] = colorArray[esp_random() % NUM_COLORS];
//         }

//         leds.setColor(LED_FRONT_LEFT, colors[0]);
//         leds.setColor(LED_FRONT_RIGHT, colors[1]);
//         leds.setColor(LED_BACK_LEFT, colors[2]);
//         leds.setColor(LED_BACK_RIGHT, colors[3]);

//         delay(1000);
//     }
// }

void ledTask(void* params) {
    using namespace COLORS;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ioShieldFlex = ctx->ioShieldFlex;
    Leds &leds = ctx->leds;
    BatteryManager &batteryManager = ctx->batteryManager;

    rgb_color_t colorArray[8] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };

    // Animation step counters
    uint8_t step_random = 0;
    uint8_t step_breath = 0;
    uint8_t step_police = 0;
    uint8_t step_warning = 0;
    uint8_t step_battery = 0;

    // Last update timestamps (µs)
    uint64_t last_random = 0;
    uint64_t last_breath = 0;
    uint64_t last_police = 0;
    uint64_t last_warning = 0;
    uint64_t last_battery = 0;

    // Intervals
    const uint64_t INTERVAL_RANDOM   = 300000;   // 300 ms
    const uint64_t INTERVAL_BREATH   = 10000;    // 10 ms
    const uint64_t INTERVAL_POLICE   = 200000;   // 200 ms
    const uint64_t INTERVAL_WARNING_ON  = 500000; // 500 ms ON
    const uint64_t INTERVAL_WARNING_OFF = 500000; // 500 ms OFF
    const uint64_t INTERVAL_BATTERY = 1000000;    // 1 s battery update

    // Thresholds for battery percentage
    const uint8_t P_HIGH   = 80;
    const uint8_t P_MEDIUM = 50;
    const uint8_t P_LOW    = 20;

    while (true) {
        uint8_t mode = ioShieldFlex.get<int>("leds_mode");
        uint64_t now = esp_timer_get_time();

        switch (mode) {
            // RANDOM BLINK
            case 0:
                if (now - last_random >= INTERVAL_RANDOM) {
                    last_random = now;
                    leds.setColor(LED_FRONT_LEFT, colorArray[esp_random() % 8]);
                    leds.setColor(LED_FRONT_RIGHT, colorArray[esp_random() % 8]);
                    leds.setColor(LED_BACK_LEFT, colorArray[esp_random() % 8]);
                    leds.setColor(LED_BACK_RIGHT, colorArray[esp_random() % 8]);
                }
                break;

            // BREATHING
            case 1:
                if (now - last_breath >= INTERVAL_BREATH) {
                    last_breath = now;
                    uint8_t fade = (step_breath < 128) ? step_breath * 2 : (255 - (step_breath - 128) * 2);
                    rgb_color_t c = rgb_color_t{fade, fade, fade};
                    leds.setAll(c);
                    step_breath = (step_breath + 1) % 256;
                }
                break;

            // POLICE FLASH
            case 2:
                if (now - last_police >= INTERVAL_POLICE) {
                    last_police = now;
                    bool leftBlue = (step_police % 2 == 0);
                    bool rightRed = (step_police % 2 == 1);

                    leds.setColor(LED_FRONT_LEFT, leftBlue  ? BLUE : OFF);
                    leds.setColor(LED_BACK_LEFT, leftBlue  ? BLUE : OFF);
                    leds.setColor(LED_FRONT_RIGHT, rightRed  ? RED  : OFF);
                    leds.setColor(LED_BACK_RIGHT, rightRed  ? RED  : OFF);
                    step_police++;
                }
                break;

            // WARNING FLASHER
            case 3:
                {
                    bool warning_on = (step_warning % 2 == 0);
                    if ((warning_on && now - last_warning >= INTERVAL_WARNING_ON) ||
                        (!warning_on && now - last_warning >= INTERVAL_WARNING_OFF)) {
                        last_warning = now;
                        leds.setAll(warning_on ? YELLOW : OFF);
                        step_warning++;
                    }
                }
                break;

            // BATTERY STATUS
            case 4:
                if (now - last_battery >= INTERVAL_BATTERY) {
                    last_battery = now;
                    uint8_t percentage = batteryManager.getPercentage();
                    rgb_color_t color = OFF;
                    bool blink = false;

                    if (percentage >= P_HIGH) {
                        color = GREEN;
                    } else if (percentage >= P_MEDIUM) {
                        color = YELLOW;
                    } else if (percentage >= P_LOW) {
                        color = RED;
                    } else {
                        color = RED;
                        blink = true;
                    }

                    if (blink) {
                        bool on = (step_battery % 2 == 0);
                        leds.setAll(on ? color : OFF);
                        step_battery++;
                    } else {
                        leds.setAll(color);
                    }
                }
                break;

            // CAR LIGHTS
            case 5:
                leds.setColor(LED_FRONT_LEFT, WHITE);
                leds.setColor(LED_FRONT_RIGHT, WHITE);
                leds.setColor(LED_BACK_LEFT, RED);
                leds.setColor(LED_BACK_RIGHT, RED);
                break;
        }

        delay(1);
    }
}