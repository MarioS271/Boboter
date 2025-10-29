// IO_SHIELD_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_lcd_panel_ssd1306.h"

class IOShield {
private:
    static constexpr uint8_t DISPLAY_I2C_ADDR = 0x3C;

    static constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_5;
    static constexpr uint8_t BUTTON_DEBOUNCE = 100;  // ms

    static constexpr uint8_t DISPLAY_WIDTH = 128;
    static constexpr uint8_t DISPLAY_HEIGHT = 64;
    static constexpr uint8_t CHAR_WIDTH = 8;
    static constexpr uint8_t CHAR_HEIGHT = 8;

    uint8_t cursorX = 0;
    uint8_t cursorY = 0;

    esp_lcd_panel_handle_t panel = nullptr;
    esp_lcd_panel_io_handle_t io_handle = nullptr;

    uint8_t displayBuffer[(DISPLAY_WIDTH * DISPLAY_HEIGHT) / 8] = {0};

public:
    explicit IOShield();

    void displayWriteText(const char* text);
    void displayClear();
    void displaySetCursorPos(uint8_t x, uint8_t y);
    void displayRefresh();

    bool getButtonState();
    bool getDebouncedButtonState();
};
