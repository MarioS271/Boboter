/**
 * @file error.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <source_location>
#include <esp_err.h>

namespace Boboter::Libs::Error {
    /**
     * @brief Checks if the given expression failed and aborts if so
     *
     * @return (void)  
     *
     * @param tag The tag to use when logging
     * @param expr The expression to check
    */
    void ERROR_CHECK(const char* tag,
                     esp_err_t expr,
                     const std::source_location loc = std::source_location::current());

    /**
     * @brief Checks if the given expression failed and warns if so
     *
     * @return (void)  
     *
     * @param tag The tag to use when logging
     * @param expr The expression to check
    */
    void WARN_CHECK(const char* tag,
                    esp_err_t expr,
                    const std::source_location loc = std::source_location::current());
}