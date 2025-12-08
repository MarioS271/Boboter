/**
 * @file web_ui.cpp
 * @authors MarioS271
 */

#include "web_ui.hpp"

#include <cstring>
#include "system_context.hpp"
#include "logger.hpp"
#include "error.hpp"

WebUI::WebUI(SystemContext* sysctx)
: server(nullptr), ctx(sysctx) {}

WebUI::~WebUI()
{
    if (server)
        httpd_stop(server);
}

esp_err_t WebUI::startServer()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ERROR_CHECK(TAG, httpd_start(&server, &config));
    ERROR_CHECK(TAG, registerRoutes());

    LOGI(TAG, "Successfully started WebUI server");
    return ESP_OK;
}

void WebUI::loop()
{}