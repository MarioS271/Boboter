/**
 * @file logger.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "logger.hpp"

#include <cstdio>
#include <cstring>
#include <cstdarg>

namespace Boboter::Libs::Logger::Internal {
    void custom_log(esp_log_level_t level,
                    const char* tag,
                    const char* format,
                    ...)
    {
        if (level > esp_log_level_get(tag)) return;
    
        va_list args, args2;
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
        int len = asprintf(&buffer, "%c [%s]: %s\n",
                            log_level_char, tag, format);
    
        if (len >= 0 && buffer != nullptr) {
            esp_log_writev(level, tag, buffer, args);
            free(buffer);
        }
    
        va_end(args);
    }
}
