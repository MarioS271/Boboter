/**
 * @file linefollower_types.hpp
 * @authors MarioS271
 */

#pragma once

#include <cstdint>

enum lf_module_t : uint8_t
{
    LF_RIGHT = 0,
    LF_LEFT = 1
};

enum lf_result_t : uint8_t
{
    LF_WHITE = 0,
    LF_BLACK = 1
};