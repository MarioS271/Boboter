/**
 * @file io_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "tasks/io_task/io_task.hpp"

#include <esp_timer.h>
#include "types/system_context.hpp"
#include "types/flex.hpp"
#include "helpers/delay.hpp"

#include "lib/battery/battery.hpp"
#include "lib/other_leds/other_leds.hpp"
#include "lib/display/display.hpp"
#include "lib/buzzer/buzzer.hpp"
#include "lib/button/button.hpp"

void Boboter::Tasks::IO::task(void* params) {
    using namespace Constants;
    using namespace Boboter::Types;
    using namespace Boboter::Libs::Display;
    using namespace Boboter::Libs::Button;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    Display& display = ctx->display;
    Buzzer& buzzer = ctx->buzzer;
    Button& button_primary = ctx->button_primary;
    Button& button_secondary = ctx->button_secondary;

    // SystemContext* ctx = static_cast<SystemContext*>(params);
    // FlexStruct &ownFlex = ctx->ioShieldFlex;
    // IOShield &ioShield = ctx->ioShield;
    // BatteryManager &batteryManager = ctx->batteryManager;

    // char buffer[32];
    // bool lf_enabled = false;
    // uint8_t leds_mode = 0;
    // uint8_t count = 10;
    // uint64_t button_pressed_time = 0;

    // ownFlex.set<int>("leds_mode", leds_mode);
    // ownFlex.set<bool>("linefollower_enabled", lf_enabled);

    // ioShield.displaySetCursorPos(3, 0);
    // ioShield.displayWriteText("=== Boboter ===");

    // // ioShield.displaySetCursorPos(0, 15);
    // // ioShield.displayWriteText("Hello, World!");

    // snprintf(buffer, sizeof(buffer), "LF: %s ", lf_enabled ? "true" : "false");
    // ioShield.displaySetCursorPos(0, 15);
    // ioShield.displayWriteText(buffer);

    // snprintf(buffer, sizeof(buffer), "LEDS: %d", leds_mode);
    // ioShield.displaySetCursorPos(0, 25);
    // ioShield.displayWriteText(buffer);

    // while (true) {
    //     count++;

    //     if (count >= 10) {
    //         count = 0;

    //         batteryManager.update();
    //         snprintf(buffer, sizeof(buffer), "Bat: %d%%  ", batteryManager.getPercentage());

    //         ioShield.displaySetCursorPos(0, 50);
    //         ioShield.displayWriteText(buffer);
    //     }

    //     if (ioShield.getButtonState()) {
    //         if (button_pressed_time == 0) button_pressed_time = esp_timer_get_time();
    //     } 
    //     else {
    //         if (button_pressed_time == 0)
    //             delay(100);
    //             continue;

    //         uint64_t press_duration = esp_timer_get_time() - button_pressed_time;

    //         if (press_duration > 500000) {  // 500 ms
    //             lf_enabled = !lf_enabled;
    //             ownFlex.set<bool>("linefollower_enabled", lf_enabled);

    //             snprintf(buffer, sizeof(buffer), "LF: %s ", lf_enabled ? "true" : "false");
    //             ioShield.displaySetCursorPos(0, 15);
    //             ioShield.displayWriteText(buffer);

    //             LOGI(TAG, "lf_enabled: %s", lf_enabled ? "true" : "false");
    //         }
    //         else {
    //             leds_mode++;
    //             if (leds_mode > 7) leds_mode = 0;
    //             ownFlex.set<int>("leds_mode", leds_mode);

    //             snprintf(buffer, sizeof(buffer), "LEDS: %d", leds_mode);
    //             ioShield.displaySetCursorPos(0, 25);
    //             ioShield.displayWriteText(buffer);

    //             LOGI(TAG, "leds_mode: %d", leds_mode);
    //         }

    //         button_pressed_time = 0;
    //     }

    //     delay(100);
    // }
}