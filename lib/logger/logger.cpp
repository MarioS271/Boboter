// LOGGER_CPP
// Boboter
// (C) MarioS271 2025

#include "logger.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>

void custom_log(esp_log_level_t level,
                const char* tag,
                const char* format,
                ...)
{
    if (level > esp_log_level_get(tag)) { return; }

    va_list args1, args2;
    va_start(args1, format);
    va_copy(args2, args1);

    char log_level_char;
    switch (level) {
        case ESP_LOG_ERROR: log_level_char = 'E'; break;
        case ESP_LOG_WARN:  log_level_char = 'W'; break;
        case ESP_LOG_INFO:  log_level_char = 'I'; break;
        case ESP_LOG_DEBUG: log_level_char = 'D'; break;
        default: log_level_char = '?'; break;
    }

    char* buffer = nullptr;
    int len = asprintf(&buffer, "## %c [%s]: %s", log_level_char, tag, format);

    if (len >= 0 && buffer != nullptr) {
        esp_log_writev(level, tag, buffer, args1);
        free(buffer);
    }

    va_end(args1);
    va_end(args2);
}   