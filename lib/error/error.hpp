/**
 * @file error.hpp
 * @authors MarioS271
 */

#pragma once

#include <esp_err.h>
#include "logger.hpp"

/**
 * @brief Callback for WARN_CHECK
 *
 * @return void
 *
 * @param tag The tag to use for logging
 * @param err The error to warn about
 * @param expr The expression where the error ocurred
 */ 
void warn_callback(const char* tag,
                   esp_err_t err,
                   const char* expr);

/**
 * @brief Callback for ERROR_CHECK
 *
 * @return void
 *
 * @param tag The tag to use for logging
 * @param err The error to handle
 * @param file The file where the error ocurred
 * @param line The line where the error ocurred
 * @param expr The expression where the error ocurred
 */
void error_callback(const char* tag,
                    esp_err_t err,
                    const char* file,
                    int line,
                    const char* expr);
            

/**
 * @brief Checks if the given expression succeeded and if not, halts
 *
 * @param tag The tag to use for logging
 * @param x The expression to check
 */
#define ERROR_CHECK(tag, x) do {                                \
    esp_err_t _err = (x);                                       \
    if (_err != ESP_OK) {                                       \
        error_callback(tag, _err, __FILE__, __LINE__, #x);      \
    }                                                           \
} while(0)

/**
 * @brief Checks if the given expression succeeded and if not, warns
 *
 * @param tag The tag to use for logging
 * @param x The expression to check
 */
#define WARN_CHECK(tag, x) do {                                 \
    esp_err_t _err = (x);                                       \
    if (_err != ESP_OK) {                                       \
        warn_callback(tag, _err, #x);                           \
    }                                                           \
} while(0)
