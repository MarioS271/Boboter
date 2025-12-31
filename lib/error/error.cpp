/**
 * @file error.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "error.h"
#include "lib/logger/logger.h"

void error_check_callback(const char* tag,
                const esp_err_t expr,
                const std::source_location loc)
{
    if (expr == ESP_OK) [[likely]]
        return;

    printf("\n");
    LOGE(tag, "ERROR_CHECK failed with code %s (0x%x)", esp_err_to_name(expr), expr);
    LOGE(tag, "  on line %u", loc.line());
    LOGE(tag, "  in file %s", loc.file_name());

    abort();
}

void warn_check_callback(const char* tag,
                const esp_err_t expr,
                const std::source_location loc)
{
    if (expr == ESP_OK) [[likely]]
        return;

    printf("\n");
    LOGW(tag, "WARN_CHECK failed with code %s (0x%x)", esp_err_to_name(expr), expr);
    LOGW(tag, "  on line %u", loc.line());
    LOGW(tag, "  in file %s", loc.file_name());
    printf("\n");
}
