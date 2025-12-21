/**
 * @file button_types.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>

namespace Boboter::Types::Button {
    enum Id : uint8_t {
        PRIMARY = 0,
        SECONDARY = 1
    };
}