/**
 * @file sound.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <cstdint>
#include <vector>
#include "types/note.h"

/**
 * @brief A datatype to store information about a sound to play on the buzzer
 */
using sound = std::vector<note>;