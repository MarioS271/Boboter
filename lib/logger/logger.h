/**
 * @file logger.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once
#include <esp_log.h>

/**
 * @brief Callback for custom logging
 *
 * @param level The log level
 * @param tag The tag to use
 * @param format The content to log (using same formatting as printf)
 */
void custom_log(esp_log_level_t level,
                const char* tag,
                const char* format,
                ...) __attribute__((format(printf, 3, 4)));

/**
 * @brief Logs a message with the verbose log level
 * @note Wrapper for @c custom_log
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c custom_log
 *
 * @param format The printf-style format to use
 */
#define LOGV(format, ...) custom_log(ESP_LOG_VERBOSE, TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs a message with the debug log level
 * @note Wrapper for @c custom_log
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c custom_log
 *
 * @param format The printf-style format to use
 */
#define LOGD(format, ...) custom_log(ESP_LOG_DEBUG, TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs a message with the information log level
 * @note Wrapper for @c custom_log
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c custom_log
 *
 * @param format The printf-style format to use
 */
#define LOGI(format, ...) custom_log(ESP_LOG_INFO, TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs a message with the warning log level
 * @note Wrapper for @c custom_log
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c custom_log
 *
 * @param format The printf-style format to use
 */
#define LOGW(format, ...) custom_log(ESP_LOG_WARN, TAG, format, ##__VA_ARGS__)

/**
 * @brief Logs a message with the error log level
 * @note Wrapper for @c custom_log
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c custom_log
 *
 * @param format The printf-style format to use
 */
#define LOGE(format, ...) custom_log(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__)