/**
 * @file error.cpp
 * @authors MarioS271
 */

#include "error.hpp"

void warn_callback(const char* tag,
                   esp_err_t err,
                   const char* expr)
{
    LOGW(tag, "%s failed with code %s (0x%x)", expr, esp_err_to_name(err), err);
}


void error_callback(const char* tag,
                    esp_err_t err,
                    const char* file,
                    int line,
                    const char* expr)
{
    printf("\n");
    LOGE(tag, "ERROR_CHECK failed with code %s (0x%x)", esp_err_to_name(err), err);
    LOGE(tag, " > file: %s on line %d", file, line);
    LOGE(tag, " > expr: %s", expr);

    abort();
}