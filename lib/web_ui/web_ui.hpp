// WEBUI_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "web_ui_constants.hpp"
#include "web_ui_json_strings.hpp"

#include <esp_http_server.h>
#include "system_context.hpp"

class WebUI
{
private:
    static constexpr const char* TAG = "WebUI";

    httpd_handle_t server;
    SystemContext* ctx;
    
    esp_err_t registerRoutes();

    esp_err_t registerGetRoutes();
    esp_err_t registerPostRoutes();
    
public:
    explicit WebUI(SystemContext* sysctx);
    ~WebUI();

    esp_err_t startServer();
    void loop();
};