// IO_SHIELD_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "delay.h"
#include "system_context.h"
#include "flex_struct.h"

#include "io_shield.h"

void ioShieldTask(void* params) {
    SystemContext* ctx = static_cast<SystemContext*>(params);
    IOShield &ioShield = ctx->ioShield;
    Gyro &gyro = ctx->gyro;
    FlexStruct &ownFlex = ctx->ioShieldFlex;

    ioShield.displaySetCursorPos(3, 0);
    ioShield.displayWriteText("=== Boboter ===");

    gyro.calibrate();

    while (true) {
        gyro.update();

        char buf[32];

        snprintf(buf, sizeof(buf), "Pitch: %.1f", gyro.getPitch());
        ioShield.displaySetCursorPos(0, 15);
        ioShield.displayWriteText(buf);

        snprintf(buf, sizeof(buf), "Yaw: %.1f", gyro.getYaw());
        ioShield.displaySetCursorPos(0, 25);
        ioShield.displayWriteText(buf);

        snprintf(buf, sizeof(buf), "Roll: %.1f", gyro.getRoll());
        ioShield.displaySetCursorPos(0, 35);
        ioShield.displayWriteText(buf);

        delay(250);
    }

    // ioShield.displaySetCursorPos(0, 10);
    // ioShield.displayWriteText("Hello, World!");

    // long i = 0;
    // while (true) {
    //     i++;

    //     if (i % 100 == 0) {
    //         char buf[16];
    //         snprintf(buf, sizeof(buf), "%ld", i / 100);

    //         ioShield.displaySetCursorPos(0, 25);
    //         ioShield.displayWriteText(buf);
    //     }

    //     ownFlex.set<bool>("button_pressed", ioShield.getButtonState());

    //     delay(10);
    // }
}