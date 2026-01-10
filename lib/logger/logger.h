/**
 * @file logger.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

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
     * @param ... The varadic argument list of the format string
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
     * @note Blocks execution if queue is empty and waits for new entries, so use in a seperate task!
     */
    void process_log_queue() const;
};

#define LOGV(format, ...) Logger::get_instance().custom_log(ESP_LOG_VERBOSE, TAG, format, ##__VA_ARGS__)
#define LOGD(format, ...) Logger::get_instance().custom_log(ESP_LOG_DEBUG, TAG, format, ##__VA_ARGS__)
#define LOGI(format, ...) Logger::get_instance().custom_log(ESP_LOG_INFO, TAG, format, ##__VA_ARGS__)
#define LOGW(format, ...) Logger::get_instance().custom_log(ESP_LOG_WARN, TAG, format, ##__VA_ARGS__)
#define LOGE(format, ...) Logger::get_instance().custom_log(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__)