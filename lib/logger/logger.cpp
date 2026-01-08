/**
 * @file logger.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "logger.h"

#include <cstdio>
#include <cstdarg>
#include "include/flags.h"

static void render_to_console(const esp_log_level_t level, const char* tag, const char* buffer) {
    char log_level_char;
    const char* RESET = "\033[0m";
    const char* DARKER = "\033[0;90m";
    const char* COLOR = "";

    switch (level) {
        case ESP_LOG_ERROR:   log_level_char = 'E'; COLOR = "\033[0;31m"; break;
        case ESP_LOG_WARN:    log_level_char = 'W'; COLOR = "\033[0;33m"; break;
        case ESP_LOG_INFO:    log_level_char = 'I'; COLOR = "\033[0;36m"; break;
        case ESP_LOG_DEBUG:   log_level_char = 'D'; COLOR = "\033[0;32m"; break;
        case ESP_LOG_VERBOSE: log_level_char = 'V'; COLOR = "\033[0;35m"; break;
        default:              log_level_char = '?'; break;
    }

    if (Flags::ENABLE_COLOR_LOGGING) {
        printf("%s%c %s[%s]%s %s\n", COLOR, log_level_char, DARKER, tag, RESET, buffer);
    } else {
        printf("%c [%s] %s\n", log_level_char, tag, buffer);
    }
}

void Logger::custom_log(const esp_log_level_t level, const char* tag, const char* format, ...) const {
    va_list args;
    va_start(args, format);
    char* buffer = nullptr;

    if (vasprintf(&buffer, format, args) < 0) {
        va_end(args);
        return;
    }
    va_end(args);

    if (use_queue && queue_handle != nullptr) {
        const log_item item = { level, tag, buffer };
        if (xQueueSend(queue_handle, &item, 0) != pdPASS) {
            render_to_console(level, tag, buffer);
            free(buffer);
        }
    } else {
        render_to_console(level, tag, buffer);
        free(buffer);
    }
}

void Logger::switch_to_queue_logging(const QueueHandle_t log_queue_handle) {
    queue_handle = log_queue_handle;
    use_queue = true;
}

void Logger::process_log_queue() const {
    if (!queue_handle) return;

    log_item item{};
    if (xQueueReceive(queue_handle, &item, portMAX_DELAY) == pdPASS) {
        render_to_console(item.level, item.tag, item.message);
        free(item.message);
    }
}
