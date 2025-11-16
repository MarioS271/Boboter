// LOGGER_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "esp_log.h"

void custom_log(esp_log_level_t level,
                const char* tag,
                const char* format,
                ...);

#define LOGD(tag, format, ...) custom_log(ESP_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...) custom_log(ESP_LOG_INFO, tag, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...) custom_log(ESP_LOG_WARN, tag, format, ##__VA_ARGS__)
#define LOGE(tag, format, ...) custom_log(ESP_LOG_ERROR, tag, format, ##__VA_ARGS__)