// IO_SHIELD_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "delay.h"
#include "system_context.h"
#include "flex_struct.h"

#include "check_battery.h"

#include "io_shield.h"


void ioShieldTask(void* params) {
    SystemContext* ctx = static_cast<SystemContext*>(params);
    IOShield &ioShield = ctx->ioShield;
    Gyro &gyro = ctx->gyro;
    FlexStruct &ownFlex = ctx->ioShieldFlex;

    ioShield.displaySetCursorPos(3, 0);
    ioShield.displayWriteText("=== Boboter ===");

    gyro.calibrate();

    uint8_t runs = 0;
    while (true) {
        gyro.update();

        char buffer[32];

        snprintf(buffer, sizeof(buffer), "Pitch: %.1f", gyro.getPitch());
        ioShield.displaySetCursorPos(0, 15);
        ioShield.displayWriteText(buffer);

        snprintf(buffer, sizeof(buffer), "Yaw: %.1f", gyro.getYaw());
        ioShield.displaySetCursorPos(0, 25);
        ioShield.displayWriteText(buffer);

        snprintf(buffer, sizeof(buffer), "Roll: %.1f", gyro.getRoll());
        ioShield.displaySetCursorPos(0, 35);
        ioShield.displayWriteText(buffer);

        delay(250);

        runs++;
        if (runs > 3) {
            runs = 0;
            snprintf(buffer, sizeof(buffer), "Bat: %d%%  ", check_battery_percentage());
            ioShield.displaySetCursorPos(0, 50);
            ioShield.displayWriteText(buffer);
        }
    }
}