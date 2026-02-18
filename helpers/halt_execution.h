/**
 * @file halt_execution.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <freertos/FreeRTOS.h>
#include "helpers/delay.h"

inline void halt_execution() {
    delay(100);
    abort();
}