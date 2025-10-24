// LED_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "esp_random.h"
#include "delay.h"
#include "system_context.h"

#include "leds.h"
#include "rgb_color.h"
#include "predef_colors.h"

using namespace COLORS;

void ledTask(void* params) {
    SystemContext* ctx = static_cast<SystemContext*>(params);
    Leds &leds = ctx->leds;

    rgb_color_t colorArray[] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
    constexpr int numColors = sizeof(colorArray) / sizeof(colorArray[0]);
    rgb_color_t colors[4] = {};

    while (true) {
        for (int i = 0; i < 4; i++) {
            colors[i] = colorArray[esp_random() % numColors];
        }

        leds.setColor(LED_FRONT_LEFT, colors[0]);
        leds.setColor(LED_FRONT_RIGHT, colors[1]);
        leds.setColor(LED_BACK_LEFT, colors[2]);
        leds.setColor(LED_BACK_RIGHT, colors[3]);

        delay(1000);
    }
}