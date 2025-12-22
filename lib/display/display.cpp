/**
 * @file display.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "display.hpp"

#include <esp_lcd_panel_ops.h>
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Display {
    Display::Display()
    : cursor_x(0),
      cursor_y(0),
      panel_handle(nullptr),
      io_handle(nullptr)
    {
        using namespace Config;
        using namespace Boboter::Libs::Logger;

        display_buffer = new uint8_t[WIDTH * HEIGHT / 8]();

        LOGI(TAG, "Initialized Display");
    }

    Display::~Display() {
        delete[] display_buffer;
    }

    void Display::clear() {
        using namespace Config;
        using namespace Boboter::Libs::Error;

        std::fill_n(display_buffer, WIDTH * HEIGHT / 8, 0);
        WARN_CHECK(TAG, esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, WIDTH, HEIGHT, display_buffer));

        cursor_x = 0;
        cursor_y = 0;
    }

    void Display::write_text(std::string text) {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        if (text.empty()) {
            LOGW(TAG, "Recieved empty string, skipping write");
            return;
        }

        if (!panel_handle) {
            LOGE(TAG, "Panel handle is nullptr!");
            return;
        }

        uint8_t x = cursor_x;
        uint8_t y = cursor_y;

        for (char c : text) {
            uint8_t glyph = static_cast<uint8_t>(c);

            if (glyph >= 128)
                glyph = '?';

            if (x + CHAR_WIDTH > WIDTH) {
                x = 0;
                y += CHAR_HEIGHT;
            }

            if (y + CHAR_HEIGHT > HEIGHT)
                break;

            uint32_t buffer_base = (y / 8) * WIDTH + x;

            for (uint8_t col = 0; col < CHAR_WIDTH; ++col)
                display_buffer[buffer_base + col] = font8x8[glyph][col];

            x += CHAR_WIDTH;
        }

        WARN_CHECK(TAG, esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, WIDTH, HEIGHT, display_buffer));
    }

    void Display::set_cursor_pos(uint8_t x, uint8_t y) {
        using namespace Config;

        if (x >= WIDTH) x = WIDTH - CHAR_WIDTH;
        if (y >= HEIGHT) y = HEIGHT - CHAR_HEIGHT;

        cursor_x = x;
        cursor_y = y;
    }
}