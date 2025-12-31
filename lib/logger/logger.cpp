/**
 * @file logger.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "logger.h"

#include <cstdio>
#include <cstdarg>

void custom_log(esp_log_level_t level,
                const char* tag,
                const char* format,
                ...)
{
    if (level > esp_log_level_get(tag)) return;

    va_list args;
    va_start(args, format);

    char log_level_char;
    switch (level) {
        case ESP_LOG_ERROR: log_level_char = 'E'; break;
        case ESP_LOG_WARN: log_level_char = 'W'; break;
        case ESP_LOG_INFO: log_level_char = 'I'; break;
        case ESP_LOG_DEBUG: log_level_char = 'D'; break;
        case ESP_LOG_VERBOSE: log_level_char = 'V'; break;
        default: log_level_char = '?'; break;
    }

    char* buffer = nullptr;

    if (vasprintf(&buffer, format, args) >= 0)
        esp_log_write(level, tag, "%c [%s]: %s\n", log_level_char, tag, buffer);

    free(buffer);

    va_end(args);
}