// WEBUI_CPP
// Boboter
// (C) MarioS271 2025

#include "webui.h"
#include "webui_constants.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"

#define TAG "WEBUI"

// Constructor
WebUI::WebUI()
: server(nullptr),
  led_upper_left(nullptr),
  led_upper_right(nullptr),
  led_lower_left(nullptr),
  led_lower_right(nullptr)
{}

// Serve the HTML page
esp_err_t WebUI::indexHandler(httpd_req_t* req) {
    httpd_resp_send(req, HTML_PAGE, strlen(HTML_PAGE));
    return ESP_OK;
}

// Handle RGB updates
esp_err_t WebUI::variablesHandler(httpd_req_t* req) {
    char buf[128];
    int len = httpd_req_get_url_query_len(req) + 1;
    if (len > 1) {
        httpd_req_get_url_query_str(req, buf, len);

        char ledId[4];
        int r = 0, g = 0, b = 0, brightness = 255;

        if (httpd_query_key_value(buf, "led", ledId, sizeof(ledId)) != ESP_OK) return ESP_FAIL;
        if (httpd_query_key_value(buf, "r", buf, sizeof(buf)) == ESP_OK) r = atoi(buf);
        if (httpd_query_key_value(buf, "g", buf, sizeof(buf)) == ESP_OK) g = atoi(buf);
        if (httpd_query_key_value(buf, "b", buf, sizeof(buf)) == ESP_OK) b = atoi(buf);
        if (httpd_query_key_value(buf, "brightness", buf, sizeof(buf)) == ESP_OK) brightness = atoi(buf);

        float scale = brightness / 255.0f;
        r = static_cast<int>(r * scale);
        g = static_cast<int>(g * scale);
        b = static_cast<int>(b * scale);

        rgb_color_t* target = nullptr;
        if (strcmp(ledId,"ul")==0) target = led_upper_left;
        else if (strcmp(ledId,"ur")==0) target = led_upper_right;
        else if (strcmp(ledId,"ll")==0) target = led_lower_left;
        else if (strcmp(ledId,"lr")==0) target = led_lower_right;

        if (target) {
            target->r = static_cast<uint8_t>(r);
            target->g = static_cast<uint8_t>(g);
            target->b = static_cast<uint8_t>(b);
        }
    }

    httpd_resp_send(req, "OK", 2);
    return ESP_OK;
}

// Start WiFi and Web server
esp_err_t WebUI::startServer() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t* sta_if = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {};
    strcpy((char*) wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char*) wifi_config.sta.password, WIFI_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Connecting to WiFi SSID: %s", WIFI_SSID);

    // Warten bis IP erhalten wird
    esp_netif_ip_info_t ipInfo;
    int retry = 0;
    const int max_retry = 20;
    while (retry < max_retry) {
        if (esp_netif_get_ip_info(sta_if, &ipInfo) == ESP_OK && ipInfo.ip.addr != 0) {
            ESP_LOGI(TAG, "WiFi connected, IP: " IPSTR, IP2STR(&ipInfo.ip));
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
        retry++;
    }
    if (retry == max_retry) {
        ESP_LOGE(TAG, "Failed to get IP address");
        return ESP_FAIL;
    }

    // Webserver starten
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        return ESP_FAIL;
    }

    httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = indexHandler,
        .user_ctx = nullptr
    };
    httpd_register_uri_handler(server, &root_uri);

    httpd_uri_t var_uri = {
        .uri = "/variables",
        .method = HTTP_GET,
        .handler = [](httpd_req_t* req) -> esp_err_t {
            WebUI* instance = (WebUI*) req->user_ctx;
            return instance->variablesHandler(req);
        },
        .user_ctx = this
    };
    httpd_register_uri_handler(server, &var_uri);

    return ESP_OK;
}
