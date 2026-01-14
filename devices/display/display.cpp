/**
 * @file display.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "display.h"

#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_io.h>
#include "include/robot.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace Device {
    Display::Display(Robot& robot) :
        robot(robot),
        display_buffer(nullptr),
        cursor_x(0),
        cursor_y(0),
        panel_handle(nullptr),
        io_handle(nullptr)
    {
        LOGI("Constructor of Device::Display called");
    }

    Display::~Display() {
        LOGI("Destructor of Device::Display called");
        shutdown();
    }

    void Display::initialize() {
        display_buffer = new uint8_t[WIDTH * HEIGHT / 8]();

        const esp_lcd_panel_io_i2c_config_t io_config = {
            .dev_addr = I2C_ADDRESS,
            .control_phase_bytes = 1,
            .dc_bit_offset = 6,
            .lcd_cmd_bits = 8,
            .lcd_param_bits = 8,
            .scl_speed_hz = 400'000
        };
        ERROR_CHECK(esp_lcd_new_panel_io_i2c(robot.i2c.get_bus_handle(), &io_config, &io_handle));

        const esp_lcd_panel_dev_config_t panel_config = {
            .bits_per_pixel = 1
        };
        ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));

        ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
        ERROR_CHECK(esp_lcd_panel_init(panel_handle));
        ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
        ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, true));

        LOGI("Initialized Device::Display");

        clear();
    }

    void Display::shutdown() {
        clear();
        WARN_CHECK(esp_lcd_panel_disp_on_off(panel_handle, false));

        if (panel_handle != nullptr) {
            esp_lcd_panel_del(panel_handle);
            panel_handle = nullptr;
        }
        if (io_handle != nullptr) {
            esp_lcd_panel_io_del(io_handle);
            io_handle = nullptr;
        }

        delete[] display_buffer;
        display_buffer = nullptr;
    }

    void Display::clear() {
        if (panel_handle == nullptr) {
            LOGW("Unable to write to display, invalid panel handle (panel not initialized?)");
            return;
        }

        std::fill_n(display_buffer, WIDTH * HEIGHT / 8, 0);
        WARN_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, WIDTH, HEIGHT, display_buffer));

        cursor_x = 0;
        cursor_y = 0;
    }

    void Display::write_text(const char* text) {
        if (panel_handle == nullptr) {
            LOGW("Unable to write to display, invalid panel handle (panel not initialized?)");
            return;
        }

        if (strlen(text) == 0) {
            LOGW("Unable to write to display, empty text string");
            return;
        }

        uint8_t x = cursor_x;
        uint8_t y = cursor_y;

        for (short i = 0; i < strlen(text); ++i) {
            uint8_t glyph = static_cast<uint8_t>(text[i]);

            if (glyph >= 128)
                glyph = '?';

            if (x + CHAR_WIDTH > WIDTH) {
                x = 0;
                y += CHAR_HEIGHT;
            }

            if (y + CHAR_HEIGHT > HEIGHT)
                break;

            const uint32_t buffer_base = (y / 8) * WIDTH + x;

            for (uint8_t col = 0; col < CHAR_WIDTH; ++col)
                display_buffer[buffer_base + col] = font8x8[glyph][col];

            x += CHAR_WIDTH;
        }

        WARN_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, WIDTH, HEIGHT, display_buffer));
    }

    void Display::set_cursor_position(uint8_t x, uint8_t y) {
        if (x >= WIDTH) {
            x = WIDTH - CHAR_WIDTH;
        }
        if (y >= HEIGHT) {
            y = HEIGHT - CHAR_HEIGHT;
        }

        cursor_x = x;
        cursor_y = y;
    }
}
