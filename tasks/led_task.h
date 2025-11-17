// LED_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "esp_random.h"
#include "delay.h"
#include "system_context.h"
#include "flex_struct.h"

#include "leds.h"
#include "rgb_color.h"
#include "predef_colors.h"

void ledTask(void* params) {
    using namespace COLORS;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ownFlex = ctx->ledsFlex;
    Leds &leds = ctx->leds;

    ownFlex.set<char*>("mode", "random_blink");
    ownFlex.set<rgb_color_t>("led_ur", BLACK);
    ownFlex.set<rgb_color_t>("led_ul", BLACK);
    ownFlex.set<rgb_color_t>("led_lr", BLACK);
    ownFlex.set<rgb_color_t>("led_ll", BLACK);

    rgb_color_t colorArray[8] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
    rgb_color_t colors[4] = {};

    while (true) {
        for (int i = 0; i < 4; i++) {
            colors[i] = colorArray[esp_random() % NUM_COLORS];
        }

        leds.setColor(LED_FRONT_LEFT, colors[0]);
        leds.setColor(LED_FRONT_RIGHT, colors[1]);
        leds.setColor(LED_BACK_LEFT, colors[2]);
        leds.setColor(LED_BACK_RIGHT, colors[3]);

        delay(1000);
    }
}