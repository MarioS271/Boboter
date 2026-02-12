/**
 * @file error.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
*/

#pragma once

#include <source_location>
#include <esp_err.h>

/**
 * @brief Checks if the given expression failed and aborts if so
 *
 * @param tag The tag to use when logging
 * @param expr The expression to check
 */
void error_check_callback(const char *tag,
                          esp_err_t expr,
                          std::source_location loc = std::source_location::current());

/**
 * @brief Checks if the given expression failed and aborts if so
 * @note Wrapper for @c error_check_callback
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c error_check_callback
 *
 * @param expr The expression to check
 */
#define ERROR_CHECK(expr) error_check_callback(TAG, expr)




/**
 * @brief Checks if the given expression failed and warns if so
 *
 * @param tag The tag to use when logging
 * @param expr The expression to check
 */
void warn_check_callback(const char* tag,
                         esp_err_t expr,
                         std::source_location loc = std::source_location::current());

/**
 * @brief Checks if the given expression failed and warns if so
 * @note Wrapper for @c warn_check_callback
 * @attention This macro requires @c TAG to be defined in the scope where it is used. If it isn't, please directly use @c warn_check_callback
 *
 * @param expr The expression to check
 */
#define WARN_CHECK(expr) warn_check_callback(TAG, expr)