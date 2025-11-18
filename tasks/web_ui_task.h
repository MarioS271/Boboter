// WEBUI_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "delay.h"
#include "system_context.h"
#include "flex_struct.h"

#include "web_ui.h"

namespace WEB_UI_TASK {
    constexpr char* TAG = "task:WEB_UI";
}

void webuiTask(void* params) {
    using namespace WEB_UI_TASK;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ownFlex = ctx->webuiFlex;
    
    WebUI webui = WebUI(ctx);

    while (true) {
        delay(1000);
    }
}