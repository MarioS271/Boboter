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
    };

public:
    static Logger& get_instance() {
        static Logger _instance;
        return _instance;
    }

    void custom_log(esp_log_level_t level, const char* tag, const char* format, ...) const __attribute__((format(printf, 4, 5)));
    void switch_to_queue_logging(QueueHandle_t log_queue_handle);
    void process_log_queue() const;
};

#define LOGV(format, ...) Logger::get_instance().custom_log(ESP_LOG_VERBOSE, TAG, format, ##__VA_ARGS__)
#define LOGD(format, ...) Logger::get_instance().custom_log(ESP_LOG_DEBUG, TAG, format, ##__VA_ARGS__)
#define LOGI(format, ...) Logger::get_instance().custom_log(ESP_LOG_INFO, TAG, format, ##__VA_ARGS__)
#define LOGW(format, ...) Logger::get_instance().custom_log(ESP_LOG_WARN, TAG, format, ##__VA_ARGS__)
#define LOGE(format, ...) Logger::get_instance().custom_log(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__)