// IO_SHIELD_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "delay.h"
#include "system_context.h"

#include "io_shield.h"

void ioShieldTask(void* params) {
    SystemContext* ctx = static_cast<SystemContext*>(params);
    IOShield &ioShield = ctx->ioShield;

    ioShield.displayWriteText(" === Boboter ===");
    ioShield.displaySetCursorPos(0, 10);
    ioShield.displayWriteText("Hello, World!");

    long i = 0;
    while (true) {
        i++;

        char buf[16];
        snprintf(buf, sizeof(buf), "%ld", i);

        ioShield.displaySetCursorPos(0, 25);
        ioShield.displayWriteText(buf);

        delay(1000);
    }
}