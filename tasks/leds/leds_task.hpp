/**
 * @file leds_task.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <esp_random.h>
#include <esp_timer.h>
#include "types/system_context.hpp"
#include "types/flex_struct.hpp"
#include "types/rgb_color.hpp"
#include "helpers/predef_colors.hpp"
#include "helpers/delay.hpp"
#include "lib/battery/battery.hpp"
#include "lib/leds/leds.hpp"

namespace Boboter::Task::Leds
{
    namespace Constants
    {}

    void Task(void* params);
}


// void ledTask(void* params) {
//     using namespace Colors;

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
//         for (int i = 0; i < 4; i++)
//             colors[i] = colorArray[esp_random() % NUM_COLORS];

//         leds.setColor(LED_FRONT_LEFT, colors[0]);
//         leds.setColor(LED_FRONT_RIGHT, colors[1]);
//         leds.setColor(LED_BACK_LEFT, colors[2]);
//         leds.setColor(LED_BACK_RIGHT, colors[3]);

//         delay(1000);
//     }
// }