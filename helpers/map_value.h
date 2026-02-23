/**
 * @file map_value.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <algorithm> // Required for std::clamp

/**
 * @brief Maps an int32_t value of a range of values to another range of values
 *
 * @return The mapped value as an int32_t
 *
 * @param x Value to map (from input range)
 * @param in_min Lower bound of input range (inclusive)
 * @param in_max Upper bound of input range (inclusive)
 * @param out_min Lower bound of output range (inclusive)
 * @param out_max Upper bound of output range (inclusive)
 */
inline int32_t map_value(const int32_t x, const int32_t in_min, const int32_t in_max, const int32_t out_min, const int32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Maps a float value from one range to an uint16_t value in another range.
 *
 * @return The mapped value as uint16_t
 *
 * @param x Value to map (from input float range)
 * @param in_min Lower bound of input float range (inclusive)
 * @param in_max Upper bound of input float range (inclusive)
 * @param out_min Lower bound of output uint16_t range (inclusive)
 * @param out_max Upper bound of output uint16_t range (inclusive)
 */
inline uint16_t map_value(const float x, const float in_min, const float in_max, const uint16_t out_min, const uint16_t out_max) {
    // Perform mapping using float arithmetic
    float result = (x - in_min) * (static_cast<float>(out_max) - static_cast<float>(out_min)) / (in_max - in_min) + static_cast<float>(out_min);

    // Clamp the result to the output range to handle potential floating point errors or
    // input 'x' being slightly outside [in_min, in_max].
    // Then cast to uint16_t.
    return static_cast<uint16_t>(std::clamp(result, static_cast<float>(out_min), static_cast<float>(out_max)));
}