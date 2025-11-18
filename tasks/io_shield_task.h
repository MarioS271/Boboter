// IO_SHIELD_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "delay.h"
#include "system_context.h"
#include "flex_struct.h"

#include "battery.h"

#include "io_shield.h"

void ioShieldTask(void* params) {
    SystemContext* ctx = static_cast<SystemContext*>(params);
    IOShield &ioShield = ctx->ioShield;
    BatteryManager &batteryManager = ctx->batteryManager;
    FlexStruct &ownFlex = ctx->ioShieldFlex;

    ioShield.displaySetCursorPos(3, 0);
    ioShield.displayWriteText("=== Boboter ===");

    ioShield.displaySetCursorPos(0, 15);
    ioShield.displayWriteText("Hello, World!");

    char buffer[32];
    while (true) {
        batteryManager.update();
        snprintf(buffer, sizeof(buffer), "Bat: %d%%  ", batteryManager.getPercentage());

        ioShield.displaySetCursorPos(0, 50);
        ioShield.displayWriteText(buffer);

        delay(1000);
    }
}