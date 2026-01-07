/**
 * @file logger.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "logger.h"

#include <cstdio>
#include <cstdarg>

void custom_log(const esp_log_level_t level,
                const char* tag,
                const char* format,
                ...)
{
    va_list args;
    va_start(args, format);

    char log_level_char;
    const char* RESET = "\033[0m";
    const char* DARKER = "\033[0;90m";
    const char* COLOR = "";

    switch (level) {
        case ESP_LOG_ERROR:
            log_level_char = 'E';
            COLOR = "\033[0;31m";  // Red
            break;

        case ESP_LOG_WARN:
            log_level_char = 'W';
            COLOR = "\033[0;33m";  // Yellow
            break;

        case ESP_LOG_INFO:
            log_level_char = 'I';
            COLOR = "\033[0;36m";  // Cyan
            break;

        case ESP_LOG_DEBUG:
            log_level_char = 'D';
            COLOR = "\033[0;32m";  // Green
            break;

        case ESP_LOG_VERBOSE:
            log_level_char = 'V';
            COLOR = "\033[0;35m";  // Magenta
            break;

        default:
            log_level_char = '?';
            break;
    }

    char* buffer = nullptr;

    if (vasprintf(&buffer, format, args) >= 0) {
        printf("%s%c %s[%s]%s %s\n", COLOR, log_level_char, DARKER, tag, RESET, buffer);
    }

    free(buffer);
    va_end(args);
}