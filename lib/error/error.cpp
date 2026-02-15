/**
 * @file error.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "error.h"
#include "lib/logger/logger.h"

void error_check_callback(const char* tag,
                          const esp_err_t expr,
                          const std::source_location loc)
{
    if (expr == ESP_OK) [[likely]]
        return;

    const Logger& logger = Logger::get_instance();

    logger.print_linefeed();
    logger.custom_log(ESP_LOG_ERROR, tag, "ERROR_CHECK failed with code %s (0x%x)", esp_err_to_name(expr), expr);
    logger.custom_log(ESP_LOG_ERROR, tag, "  on line %lu", loc.line());
    logger.custom_log(ESP_LOG_ERROR, tag, "  in file %s", loc.file_name());

    abort();
}

void warn_check_callback(const char* tag,
                         const esp_err_t expr,
                         const std::source_location loc)
{
    if (expr == ESP_OK) [[likely]]
        return;

    const Logger& logger = Logger::get_instance();

    logger.custom_log(ESP_LOG_WARN, tag, "WARN_CHECK failed with code %s (0x%x)", esp_err_to_name(expr), expr);
    logger.custom_log(ESP_LOG_WARN, tag, "  on line %lu", loc.line());
    logger.custom_log(ESP_LOG_WARN, tag, "  in file %s", loc.file_name());
}
