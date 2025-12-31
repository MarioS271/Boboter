/**
 * @file map.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

/**
 * @brief Maps a value of a range of values to another range of values
 *
 * @return The mapped value
 *
 * @param x Value to map (from input range)
 * @param in_min Lower bound of input range (inclusive)
 * @param in_max Upper bound of input range (inclusive)
 * @param out_min Lower bound of output range (inclusive)
 * @param out_max Upper bound of output range (inclusive)
 */
inline long map_value(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}