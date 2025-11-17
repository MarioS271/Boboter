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
    FlexStruct &ownFlex = ctx->ioShieldFlex;

    ioShield.displaySetCursorPos(3, 0);
    ioShield.displayWriteText("=== Boboter ===");

    ioShield.displaySetCursorPos(0, 15);
    ioShield.displayWriteText("Hello, World!");

    while (true) {
        delay(1000);
    }
}