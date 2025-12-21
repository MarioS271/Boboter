/**
 * @file note.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types {
    /**
     * @struct note
     * @brief This struct represents a musical note, used for the buzzer.
     * @note The duration is given in milliseconds!
    */
    struct note {
        uint16_t frequency;
        uint16_t duration;
    };
}