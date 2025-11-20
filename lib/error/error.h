// ERROR_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <esp_err.h>
#include "logger.h"

void warn_callback(const char* tag,
                   esp_err_t err,
                   const char* expr);

void error_callback(const char* tag,
                    esp_err_t err,
                    const char* file,
                    int line,
                    const char* expr);


#define ERROR_CHECK(tag, x) do {                                \
    esp_err_t _err = (x);                                       \
    if (_err != ESP_OK) {                                       \
        error_callback(tag, _err, __FILE__, __LINE__, #x);      \
    }                                                           \
} while(0)

#define WARN_CHECK(tag, x) do {                                 \
    esp_err_t _err = (x);                                       \
    if (_err != ESP_OK) {                                       \
        warn_callback(tag, _err, #x);                           \
    }                                                           \
} while(0)
