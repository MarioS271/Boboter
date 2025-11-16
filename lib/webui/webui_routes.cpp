// WEBUI_ROUTES_CPP
// Boboter
// (C) MarioS271 2025

#include "webui.h"
#include "webui_constants.h"

#include <cstring>
#include "esp_http_server.h"
#include "rgb_color.h"
#include "logger.h"
#include "leds.h"

#define TAG "WEBUI_ROUTES"

esp_err_t WebUI::registerRoutes() {
    /**
     * Root URI
     * Serves the main HTML page
     */
    httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = [](httpd_req_t *req) -> esp_err_t {
            httpd_resp_send(req, HTML_PAGE, HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        },
        .user_ctx = nullptr
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &root_uri));

    /**
     * LED Get URI
     * Retrieves current LED state
     * 
     * /led/get/{led_id}
     * LED IDs: ul, ur, ll, lr
     */
    httpd_uri_t led_get_uri = {
        .uri = "/led/get/*",
        .method = HTTP_GET,
        .handler = [](httpd_req_t *req) -> esp_err_t {
            const char* uri = req->uri;
            const char* led_id = uri + strlen("/led/get/");

            led_pos_t led_pos;
            if (strcmp(led_id, "ul") == 0) {
                led_pos = LED_FRONT_LEFT;
            } else if (strcmp(led_id, "ur") == 0) {
                led_pos = LED_FRONT_RIGHT;
            } else if (strcmp(led_id, "ll") == 0) {
                led_pos = LED_BACK_LEFT;
            } else if (strcmp(led_id, "lr") == 0) {
                led_pos = LED_BACK_RIGHT;
            } else {
                httpd_resp_send_404(req);
                return ESP_OK;
            }

            WebUI* self = static_cast<WebUI*>(req->user_ctx);
            if (!self) {
                return ESP_FAIL;
            }

            rgb_color_t color;
            self->ctx->leds.getColor(led_pos, color);

            char resp[64];
            snprintf(resp, sizeof(resp), "{\"led\":\"%s\",\"r\":%d,\"g\":%d,\"b\":%d}", led_id, color.r, color.g, color.b);
            httpd_resp_set_type(req, "application/json");
            httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

            return ESP_OK;
        },
        .user_ctx = this
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &led_get_uri));

    /**
     * LED Set URI
     * Sets LED state
     * 
     * /led/set/{led_id}?r={red}&g={green}&b={blue}
     * LED IDs: ul, ur, ll, lr
     * Uses RGB Color Values (0-255)
     * 
     * TODO: Implement LED control logic
     */
    httpd_uri_t led_set_uri = {
        .uri = "/led/set/*",
        .method = HTTP_POST,
        .handler = [](httpd_req_t *req) -> esp_err_t {
            // Place LED logic here
            return ESP_OK;
        },
        .user_ctx = nullptr
    };
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &led_set_uri));
}
