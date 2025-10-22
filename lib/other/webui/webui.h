// WEBUI_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "esp_http_server.h"
#include "rgb_color.h"

class WebUI {
private:
    httpd_handle_t server;

    static esp_err_t indexHandler(httpd_req_t* request);
    esp_err_t variablesHandler(httpd_req_t* request);
    
public:
    explicit WebUI();

    rgb_color_t* led_upper_left;
    rgb_color_t* led_upper_right;
    rgb_color_t* led_lower_left;
    rgb_color_t* led_lower_right;

    esp_err_t startServer();
};