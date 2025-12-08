/**
 * @file web_ui_task.hpp
 * @authors MarioS271
 */

#pragma once

#include "system_context.hpp"
#include "flex_struct.hpp"
#include "delay.hpp"
#include "web_ui.hpp"

namespace WebUI_Task 
{
    constexpr const char* TAG = "task:WebUI";
}

void webuiTask(void* params)
{
    using namespace WebUI_Task;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ownFlex = ctx->webuiFlex;
    
    WebUI webui = WebUI(ctx);

    while (true)
    {
        delay(1000);
    }
}