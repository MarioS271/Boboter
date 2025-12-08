/**
 * @file io_shield_task.hpp
 * @authors MarioS271
 */

#pragma once

#include <esp_timer.h>
#include "system_context.hpp"
#include "flex_struct.hpp"
#include "delay.hpp"
#include "battery.hpp"
#include "io_shield.hpp"

namespace IOShield_Task
{
    constexpr const char* TAG = "task:IOShield";
}

void ioShieldTask(void* params)
{
    using namespace IOShield_Task;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ownFlex = ctx->ioShieldFlex;
    IOShield &ioShield = ctx->ioShield;
    BatteryManager &batteryManager = ctx->batteryManager;

    char buffer[32];
    bool lf_enabled = false;
    uint8_t leds_mode = 0;
    uint8_t count = 10;
    uint64_t button_pressed_time = 0;

    ownFlex.set<int>("leds_mode", leds_mode);
    ownFlex.set<bool>("linefollower_enabled", lf_enabled);

    ioShield.displaySetCursorPos(3, 0);
    ioShield.displayWriteText("=== Boboter ===");

    // ioShield.displaySetCursorPos(0, 15);
    // ioShield.displayWriteText("Hello, World!");

    snprintf(buffer, sizeof(buffer), "LF: %s ", lf_enabled ? "true" : "false");
    ioShield.displaySetCursorPos(0, 15);
    ioShield.displayWriteText(buffer);

    snprintf(buffer, sizeof(buffer), "LEDS: %d", leds_mode);
    ioShield.displaySetCursorPos(0, 25);
    ioShield.displayWriteText(buffer);

    while (true)
    {
        count++;

        if (count >= 10)
        {
            count = 0;

            batteryManager.update();
            snprintf(buffer, sizeof(buffer), "Bat: %d%%  ", batteryManager.getPercentage());

            ioShield.displaySetCursorPos(0, 50);
            ioShield.displayWriteText(buffer);
        }

        if (ioShield.getButtonState())
        {
            if (button_pressed_time == 0) button_pressed_time = esp_timer_get_time();
        } 
        else
        {
            if (button_pressed_time == 0)
                continue;

            uint64_t press_duration = esp_timer_get_time() - button_pressed_time;

            if (press_duration > 500000)  // 500 ms
            {
                lf_enabled = !lf_enabled;
                ownFlex.set<bool>("linefollower_enabled", lf_enabled);

                snprintf(buffer, sizeof(buffer), "LF: %s ", lf_enabled ? "true" : "false");
                ioShield.displaySetCursorPos(0, 15);
                ioShield.displayWriteText(buffer);

                LOGI(TAG, "lf_enabled: %s", lf_enabled ? "true" : "false");
            }
            else
            {
                leds_mode++;
                if (leds_mode > 7) leds_mode = 0;
                ownFlex.set<int>("leds_mode", leds_mode);

                snprintf(buffer, sizeof(buffer), "LEDS: %d", leds_mode);
                ioShield.displaySetCursorPos(0, 25);
                ioShield.displayWriteText(buffer);

                LOGI(TAG, "leds_mode: %d", leds_mode);
            }

            button_pressed_time = 0;
        }

        delay(100);
    }
}