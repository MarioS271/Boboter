/**
 * @file note.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once
#include <cstdint>

/**
 * @brief Represents a musical note, used for the buzzer
 */
struct note {
    uint16_t frequency;   ///< Frequency in Hz
    uint16_t duration;    ///< Duration in ms
};