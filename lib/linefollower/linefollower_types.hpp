/**
 * @file linefollower_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::Linefollower {
    enum Id : uint8_t {
        RIGHT = 0,
        LEFT = 1
    };
    
    enum Result : uint8_t {
        WHITE = 0,
        BLACK = 1
    };
}