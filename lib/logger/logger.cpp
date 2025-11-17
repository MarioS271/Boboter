// LOGGER_CPP
// Boboter
// (C) MarioS271 2025

#include "logger.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include "ansi_colors.h"

using namespace ANSI_COLORS;

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

    const char* log_level_color = nullptr;
    switch (level) {
        case ESP_LOG_ERROR: log_level_color = RED; break;
        case ESP_LOG_WARN: log_level_color = YELLOW; break;
        case ESP_LOG_INFO: log_level_color = GREEN; break;
        case ESP_LOG_DEBUG: log_level_color = CYAN; break;
        default: log_level_color = WHITE; break;
    }

    char* buffer = nullptr;
    int len = asprintf(&buffer, "%s%s%c %s%s[%s]:%s %s",
                                BOLD, log_level_color, log_level_char,
                                RESET, BOLD, tag, RESET, format);

    if (len >= 0 && buffer != nullptr) {
        esp_log_writev(level, tag, buffer, args1);
        free(buffer);
    }

    va_end(args1);
    va_end(args2);
}   