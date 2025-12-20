/**
 * @file bumper_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::Bumper {
    enum Id : uint8_t {
        BUMPER_LEFT = 0,
        BUMPER_RIGHT = 1
    };
}