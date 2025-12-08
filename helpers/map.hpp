/**
 * @file map.hpp
 * @authors MarioS271
 */

#pragma once

inline long map_value(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}