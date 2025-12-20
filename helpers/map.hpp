/**
 * @file map.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

namespace Boboter::Helpers {
    /**
     * @brief Maps a value of a range of values to another range of values
     *
     * @return (long) The mapped value
     *
     * @param x The value from the in range to map to the out range
     * @param in_min The lower bound of the input range (inclusive)
     * @param in_max The upper bound of the input range (inclusive)
     * @param out_min The lower bound of the output range (inclusive)
     * @param out_max The upper bound of the output range (inclusive)
    */
    inline long map_value(long x, long in_min, long in_max, long out_min, long out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
}