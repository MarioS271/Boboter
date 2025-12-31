/**
 * @file check_for_nullopt.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once
#include <optional>

#define CHECK_FOR_NULLOPT(value) if (!value.has_value()) { \
    LOGE("Recieved std::nullopt");                         \
    abort();                                               \
}