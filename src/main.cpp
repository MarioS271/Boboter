/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/constants.h"
#include "include/flags.h"
#include "include/robot.h"
#include "types/protected_struct.h"

extern "C" void app_main() {
    using namespace Constants;

    struct data {
        int zahl;
        bool wert;
    };
    protected_struct<data> data;

    data->zahl = 0;
}
