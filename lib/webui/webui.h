// WEBUI_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "esp_http_server.h"
#include "system_context.h"

class WebUI {
private:
    httpd_handle_t server;
    SystemContext* ctx;
    
    esp_err_t registerRoutes();
    
public:
    explicit WebUI(SystemContext* sysctx);
    ~WebUI();

    esp_err_t startServer();
    void loop();
};