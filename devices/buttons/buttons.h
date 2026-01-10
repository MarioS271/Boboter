/**
 * @file buttons.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>

static constexpr const char* TAG = "Device::Buttons";
static constexpr gpio_num_t BUTTON_PRIMARY_PIN = GPIO_NUM_16;
static constexpr gpio_num_t BUTTON_SECONDARY_PIN = GPIO_NUM_17;
static constexpr uint32_t DEBOUNCE_US = 250'000;

namespace Device {
    class Buttons {
    private:

    };
}