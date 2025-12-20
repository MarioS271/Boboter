/**
 * @file logger.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <esp_log.h>

namespace Boboter::Libs::Logger {
    namespace Internal {
        /**
         * @brief Callback for custom logging
         *
         * @return (void)  
         *
         * @param level The log level
         * @param tag The tag to use
         * @param format The content to log (using same formatting as printf)
         */
        void custom_log(esp_log_level_t level,
                        const char* tag,
                        const char* format,
                        ...);
    }

    /**
     * @brief Logs a message with the verbose log level
     *
     * @return (void)  
     *
     * @param tag The tag to log with
     * @param format The printf-style format to use
     * @param args All values to format the output with
    */
    template <typename... T>
    inline void LOGV(const char* tag, const char* format, T... args)
    { Internal::custom_log(ESP_LOG_VERBOSE, tag, format, args...); }

    /**
     * @brief Logs a message with the debug log level
     *
     * @return (void)  
     *
     * @param tag The tag to log with
     * @param format The printf-style format to use
     * @param args All values to format the output with
    */
    template <typename... T>
    inline void LOGD(const char* tag, const char* format, T... args)
    { Internal::custom_log(ESP_LOG_DEBUG, tag, format, args...); }

    /**
     * @brief Logs a message with the information log level
     *
     * @return (void)  
     *
     * @param tag The tag to log with
     * @param format The printf-style format to use
     * @param args All values to format the output with
    */
    template <typename... T>
    inline void LOGI(const char* tag, const char* format, T... args)
    { Internal::custom_log(ESP_LOG_INFO, tag, format, args...); }

    /**
     * @brief Logs a message with the warning log level
     *
     * @return (void)  
     *
     * @param tag The tag to log with
     * @param format The printf-style format to use
     * @param args All values to format the output with
    */
    template <typename... T>
    inline void LOGW(const char* tag, const char* format, T... args)
    { Internal::custom_log(ESP_LOG_WARN, tag, format, args...); }

    /**
     * @brief Logs a message with the error log level
     *
     * @return (void)  
     *
     * @param tag The tag to log with
     * @param format The printf-style format to use
     * @param args All values to format the output with
    */
    template <typename... T>
    inline void LOGE(const char* tag, const char* format, T... args)
    { Internal::custom_log(ESP_LOG_ERROR, tag, format, args...); }
}