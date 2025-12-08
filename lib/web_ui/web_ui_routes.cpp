/**
 * @file web_ui_routes.cpp
 * @authors MarioS271
 */

#include "web_ui.hpp"

#include <cstring>
#include <esp_http_server.h>
#include "system_context.hpp"
#include "flex_struct.hpp"
#include "rgb_color.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "battery.hpp"
#include "leds.hpp"

esp_err_t WebUI::registerRoutes()
{
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
    ERROR_CHECK(TAG, httpd_register_uri_handler(server, &root_uri));

    ERROR_CHECK(TAG, registerGetRoutes());
    ERROR_CHECK(TAG, registerPostRoutes());

    return ESP_OK;
}


esp_err_t WebUI::registerGetRoutes()
{
    /**
     * Get System Status URI
     * Retrieves system status information
     * 
     * /get/system
     */
    httpd_uri_t get_status_uri = {
        .uri = "/get/system",
        .method = HTTP_GET,
        .handler = [](httpd_req_t *req) -> esp_err_t {
            SystemContext* ctx = static_cast<SystemContext*>(req->user_ctx);

            char resp[256];

            ctx->batteryManager.update();
            uint8_t bat_percent = ctx->batteryManager.getPercentage();
            uint16_t bat_voltage = ctx->batteryManager.getVoltageMv();

            const char* ip_address = "0.0.0.0";

            snprintf(resp, sizeof(resp), JSON_RESP_GET::SYSTEM_STATUS_RESPONSE,
                    bat_percent, bat_voltage, ip_address);

            httpd_resp_set_type(req, "application/json");
            httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

            return ESP_OK;
        },
        .user_ctx = ctx
    };
    ERROR_CHECK(TAG, httpd_register_uri_handler(server, &get_status_uri));

    /**
     * LED Get URI
     * Retrieves current LED state
     * 
     * /get/leds/{led_id}
     * LED IDs: ul, ur, ll, lr
     */
    httpd_uri_t led_get_uri = {
        .uri = "/get/leds/*",
        .method = HTTP_GET,
        .handler = [](httpd_req_t *req) -> esp_err_t {
            Leds* leds = static_cast<Leds*>(req->user_ctx);

            const char* uri = req->uri;
            const char* led_id = uri + strlen("/get/leds/");

            led_pos_t led_pos;
            if (strcmp(led_id, "ul") == 0)
                led_pos = LED_FRONT_LEFT;
            else if (strcmp(led_id, "ur") == 0)
                led_pos = LED_FRONT_RIGHT;
            else if (strcmp(led_id, "ll") == 0)
                led_pos = LED_BACK_LEFT;
            else if (strcmp(led_id, "lr") == 0)
                led_pos = LED_BACK_RIGHT;
            else{
                httpd_resp_send_404(req);
                return ESP_OK;
            }

            rgb_color_t color = leds->getColor(led_pos);

            char resp[64];
            snprintf(resp, sizeof(resp), "{\"led\":\"%s\",\"r\":%d,\"g\":%d,\"b\":%d}", led_id, color.r, color.g, color.b);
            httpd_resp_set_type(req, "application/json");
            httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

            return ESP_OK;
        },
        .user_ctx = &ctx->leds
    };
    ERROR_CHECK(TAG, httpd_register_uri_handler(server, &led_get_uri));

    return ESP_OK;
}


esp_err_t WebUI::registerPostRoutes()
{
    /**
     * LED Set URI
     * Sets LED state
     * 
     * /post/leds/{led_id}?r={red}&g={green}&b={blue}
     * LED IDs: ul, ur, ll, lr
     * Uses RGB Color Values (0-255)
     * Only works when mode is 'manual
     * 
     * /post/leds/mode?mode={mode_name}
     * Mode Names: manual, random_blink
     */
    httpd_uri_t led_set_uri = {
        .uri = "/post/leds/*",
        .method = HTTP_POST,
        .handler = [](httpd_req_t *req) -> esp_err_t {
            FlexStruct* ledsFlex = static_cast<FlexStruct*>(req->user_ctx);

            // TODO: Implement logic

            return ESP_OK;
        },
        .user_ctx = &ctx->ledsFlex
    };
    ERROR_CHECK(TAG, httpd_register_uri_handler(server, &led_set_uri));

    return ESP_OK;
}