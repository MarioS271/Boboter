/**
 * @file display.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "display.h"

#include <cstring>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_io.h>
#include "include/robot.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace Device {
    Display::Display(Robot& robot) :
        robot(robot),
        display_buffer(nullptr),
        cursor_column(0),
        cursor_row(0),
        panel_handle(nullptr),
        io_handle(nullptr)
    {
        LOGD("Constructor called");
    }

    Display::~Display() {
        LOGD("Destructor called");
        shutdown();
    }

    void Display::initialize() {
        if constexpr (!Flags::ENABLE_DISPLAY) {
            LOGW("Flags::ENABLE_DISPLAY is set to false, skipping display initialization");
            return;
        }

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

        clear();

        LOGI("Initialized Device::Display");
    }

    void Display::shutdown() {
        if constexpr (!Flags::ENABLE_DISPLAY) {
            return;
        }

        if (panel_handle == nullptr) {
            LOGW("Cannot shut down display, invalid panel handle (not initialized or disabled?)");
        }

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

        LOGI("Shut down Device::Display");
    }

    void Display::clear() {
        if constexpr (!Flags::ENABLE_DISPLAY) {
            return;
        }

        if (panel_handle == nullptr) {
            LOGW("Unable to write to display, invalid panel handle (panel not initialized?)");
            return;
        }

        memset(display_buffer, 0, WIDTH * HEIGHT / 8);
        WARN_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, WIDTH, HEIGHT, display_buffer));

        cursor_column = 0;
        cursor_row = 0;

        LOGV("Cleared display");
    }

    void Display::write_text(const char* text) {
        if constexpr (!Flags::ENABLE_DISPLAY) {
            return;
        }

        if (panel_handle == nullptr) {
            LOGW("Unable to write to display, invalid panel handle (panel not initialized?)");
            return;
        }

        if (text[0] == '\0') {
            LOGW("Unable to write to display, got empty text string");
            return;
        }

        uint8_t column = cursor_column;
        uint8_t row = cursor_row;

        for (size_t i = 0; text[i] != '\0'; ++i) {
            auto glyph = static_cast<uint8_t>(text[i]);

            if (glyph >= 128) {
                glyph = '?';
            }

            if (column >= MAX_COLUMNS) {
                column = 0;
                ++row;
            }

            if (row >= MAX_ROWS) {
                break;
            }

            const uint32_t buffer_base = (row * WIDTH) + (column * CHAR_WIDTH);

            for (uint8_t char_column = 0; char_column < CHAR_WIDTH; ++char_column) {
                display_buffer[buffer_base + char_column] = font8x8[glyph][char_column];
            }

            ++column;
        }

        cursor_column = column;
        cursor_row = row;

        LOGV("Wrote text to display: \"%s\"", text);
    }

    void Display::set_cursor_position(uint8_t column, uint8_t row) {
        if constexpr (!Flags::ENABLE_DISPLAY) {
            return;
        }

        if (panel_handle == nullptr) {
            LOGW("Unable to write to display, invalid panel handle (panel not initialized?)");
            return;
        }

        if (column >= MAX_COLUMNS) {
            column = MAX_COLUMNS - 1;
        }
        if (row >= MAX_ROWS) {
            row = MAX_ROWS - 1;
        }

        cursor_column = column;
        cursor_row = row;

        LOGV("Cursor grid position set to column=%hhu row=%hhu", column, row);
    }

    void Display::write_buffer_to_display() {
        if constexpr (!Flags::ENABLE_DISPLAY) {
            return;
        }

        if (panel_handle == nullptr) {
            LOGW("Unable to write to display, invalid panel handle (panel not initialized?)");
            return;
        }

        WARN_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, WIDTH, HEIGHT, display_buffer));
    }
}
