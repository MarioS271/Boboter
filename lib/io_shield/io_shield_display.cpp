// IO_SHIELD_DISPLAY_CPP
// Boboter
// (C) MarioS271 2025

#include "io_shield.h"

#include <cstring>
#include "logger.h"
#include "error.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

void IOShield::displayClear() {
    memset(displayBuffer, 0, sizeof(displayBuffer));

    WARN_CHECK(TAG, esp_lcd_panel_draw_bitmap(panel, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, displayBuffer));

    cursorX = 0;
    cursorY = 0;
}

void IOShield::displaySetCursorPos(uint8_t x, uint8_t y) {
    if (x >= DISPLAY_WIDTH) x = DISPLAY_WIDTH - CHAR_WIDTH;
    if (y >= DISPLAY_HEIGHT) y = DISPLAY_HEIGHT - CHAR_HEIGHT;

    cursorX = x;
    cursorY = y;
}

void IOShield::displayWriteText(const char* text) {
    if (!text || !panel) return;

    uint8_t x = cursorX;
    uint8_t y = cursorY;

    for (size_t i = 0; text[i] != '\0'; i++) {
        uint8_t c = static_cast<uint8_t>(text[i]);
        if (c >= 128) c = '?';

        if (x + CHAR_WIDTH > DISPLAY_WIDTH) {
            x = 0;
            y += CHAR_HEIGHT;
        }

        if (y + CHAR_HEIGHT > DISPLAY_HEIGHT) break;

        for (uint8_t col = 0; col < CHAR_WIDTH; ++col) {
            for (uint8_t row = 0; row < CHAR_HEIGHT; ++row) {
                uint8_t pixel = (font8x8_basic_tr[c][col] >> row) & 0x01;
                size_t index = ( (y + row) / 8 ) * DISPLAY_WIDTH + (x + col);

                if (pixel) displayBuffer[index] |=  (1 << ((y + row) % 8));
                else displayBuffer[index] &= ~(1 << ((y + row) % 8));
            }
        }

        x += CHAR_WIDTH;
    }

    WARN_CHECK(TAG, esp_lcd_panel_draw_bitmap(panel, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, displayBuffer));

    cursorX = x;
    cursorY = y;
}