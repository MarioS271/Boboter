/**
 * @file encoder_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::Encoder {
    enum Id : uint8_t {
        LEFT = 0,
        RIGHT = 1
    };
}