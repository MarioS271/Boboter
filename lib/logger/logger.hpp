/**
 * @file logger.hpp
 * @authors MarioS271
 */

#pragma once

#include <esp_log.h>

/**
 * @brief Callback for custom logging
 *
 * @return void
 *
 * @param level The log level
 * @param tag The tag to use
 * @param format The content to log (using same formatting as printf)
 */
void custom_log(esp_log_level_t level,
                const char* tag,
                const char* format,
                ...);

#define LOGV(tag, format, ...) custom_log(ESP_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#define LOGD(tag, format, ...) custom_log(ESP_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...) custom_log(ESP_LOG_INFO, tag, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...) custom_log(ESP_LOG_WARN, tag, format, ##__VA_ARGS__)
#define LOGE(tag, format, ...) custom_log(ESP_LOG_ERROR, tag, format, ##__VA_ARGS__)