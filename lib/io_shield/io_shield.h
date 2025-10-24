// IO_SHIELD_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "ssd1306/ssd1306.h"

class IOShield {
private:
    static constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3C;
    static constexpr i2c_port_t DISPLAY_I2C_PORT = I2C_NUM_0;

    static constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_5;
    static constexpr uint8_t BUTTON_DEBOUNCE = 100;  // ms

    static constexpr uint8_t DISPLAY_WIDTH = 128;
    static constexpr uint8_t DISPLAY_HEIGHT = 64;
    static constexpr uint8_t CHAR_WIDTH = 8;
    static constexpr uint8_t CHAR_HEIGHT = 8;

    uint8_t cursorX = 0;
    uint8_t cursorY = 0;

    SSD1306_t dev;

public:
    explicit IOShield();

    void displayWriteText(const char* text);
    void displayClear();
    void displaySetCursorPos(uint8_t x, uint8_t y);
    void displayRefresh();

    bool getButtonState();
    bool getDebouncedButtonState();
};
