/**
 * @file logger.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include "include/flags.h"

class Logger {
private:
    explicit Logger() = default;
    ~Logger() = default;

    QueueHandle_t queue_handle = nullptr;
    bool use_queue = false;

public:
    struct log_item {
        esp_log_level_t level;
        const char* tag;
        char* message;
        bool is_linefeed;
    };

public:
    /**
     * @brief Returns a reference to the static logger instance
     * @note The instance will be created on the first call of this function
     *
     * @return Reference to the logger object
     */
    static Logger& get_instance() {
        static Logger _instance;
        return _instance;
    }

    /**
     * @brief Either directly invokes @c render_to_console (real mode) or sends to the queue (queue mode)
     *
     * @param level The logging level to use
     * @param tag The tag to log with
     * @param format The format string to use (printf-style)
     * @param ... The variadic argument list of the format string
     */
    void custom_log(esp_log_level_t level, const char* tag, const char* format, ...) const __attribute__((format(printf, 4, 5)));

    /**
     * @brief Prints a @c \n in the currently selected mode
     */
    void print_linefeed() const;

    /**
     * @brief Switches the logger's mode to queue logging
     *
     * @param log_queue_handle The queue handle of the queue where logs will be sent to
     */
    void switch_to_queue_logging(QueueHandle_t log_queue_handle);

    /**
     * @brief Processes all entries in the log queue
     * @note Blocks execution if queue is empty and waits for new entries, so use in a separate task!
     */
    void process_log_queue() const;
};

/**
 * @brief Macro for verbose logging
 * @details Checks if the log level allows verbose logs and if the module which is attempting to log is
 *          included in @c Flags::ENABLED_VERBOSE_LOG_SOURCES
 *
 * @param format The format string to use (printf-style)
 * @param ... The variadic argument list of the format string
 */
#define LOGV(format, ...) do { \
    if constexpr (Flags::LOWEST_LOG_LEVEL >= ESP_LOG_VERBOSE) { \
        if constexpr (Flags::ENABLED_VERBOSE_LOG_SOURCES & LOG_SOURCE) { \
            Logger::get_instance().custom_log(ESP_LOG_VERBOSE, TAG, format, ##__VA_ARGS__); \
        } \
    } \
} while (0)

/**
 * @brief Macro for debug logging
 * @details Checks if the log level allows debug logs
 *
 * @param format The format string to use (printf-style)
 * @param ... The variadic argument list of the format string
 */
#define LOGD(format, ...) do { \
    if constexpr (Flags::LOWEST_LOG_LEVEL >= ESP_LOG_DEBUG) { \
        Logger::get_instance().custom_log(ESP_LOG_DEBUG, TAG, format, ##__VA_ARGS__); \
    } \
} while (0)

/**
 * @brief Macro for info logging
 * @details Checks if the log level allows info logs
 *
 * @param format The format string to use (printf-style)
 * @param ... The variadic argument list of the format string
 */
#define LOGI(format, ...) do { \
    if constexpr (Flags::LOWEST_LOG_LEVEL >= ESP_LOG_INFO) { \
        Logger::get_instance().custom_log(ESP_LOG_INFO, TAG, format, ##__VA_ARGS__); \
    } \
} while (0)

/**
 * @brief Macro for warn logging
 * @details Checks if the log level allows warn logs
 *
 * @param format The format string to use (printf-style)
 * @param ... The variadic argument list of the format string
 */
#define LOGW(format, ...) do { \
    if constexpr (Flags::LOWEST_LOG_LEVEL >= ESP_LOG_WARN) { \
        Logger::get_instance().custom_log(ESP_LOG_WARN, TAG, format, ##__VA_ARGS__); \
    } \
} while (0)

/**
 * @brief Macro for error logging
 * @details Checks if the log level allows error logs
 *
 * @param format The format string to use (printf-style)
 * @param ... The variadic argument list of the format string
 */
#define LOGE(format, ...) do { \
    if constexpr (Flags::LOWEST_LOG_LEVEL >= ESP_LOG_ERROR) { \
        Logger::get_instance().custom_log(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__); \
    } \
} while (0)
