// WEBUI_CPP
// Boboter
// (C) MarioS271 2025

#include "web_ui.h"

#include <cstring>
#include "system_context.h"
#include "logger.h"

WebUI::WebUI(SystemContext* sysctx)
: server(nullptr), ctx(sysctx) {}

WebUI::~WebUI() {
    if (server) {
        httpd_stop(server);
    }
}

esp_err_t WebUI::startServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) != ESP_OK) {
        LOGE(TAG, "Failed to start WebUI server");
        return ESP_FAIL;
    }

    if (registerRoutes() != ESP_OK) {
        LOGE(TAG, "Failed to register WebUI routes");
        return ESP_FAIL;
    }

    LOGI(TAG, "Successfully started WebUI server");
    return ESP_OK;
}

void WebUI::loop() {

}