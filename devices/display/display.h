/**
 * @file display.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <esp_lcd_panel_ssd1306.h>

class Robot;

namespace Device {
    class Display {
    private:
        static constexpr const char* TAG = "Device::Display";

        static constexpr uint8_t I2C_ADDRESS = 0x3C;
        static constexpr uint8_t WIDTH = 128;
        static constexpr uint8_t HEIGHT = 64;
        static constexpr uint8_t CHAR_WIDTH = 8;
        static constexpr uint8_t CHAR_HEIGHT = 8;

        static constexpr uint8_t font8x8[128][8] = {
            #include "display_font.inc"
        };

        Robot& robot;

        uint8_t* display_buffer;
        uint8_t cursor_x;
        uint8_t cursor_y;

        esp_lcd_panel_handle_t panel_handle;
        esp_lcd_panel_io_handle_t io_handle;

    public:
        explicit Display(Robot& robot);
        ~Display();

        /**
         * @brief Configures the LCD panel and all I2C components related to the display
         */
        void initialize();

        /**
         * @brief Shuts down the LCD panel and the according I2C device
         */
        void shutdown();

        /**
         * @brief Clears the display
         */
        void clear();

        /**
         * @brief Writes text to the display at the current cursor position
         *
         * @param text The text to write
         */
        void write_text(const char* text);

        /**
         * @brief Sets the position of the cursor on the screen
         *
         * @param x The x coordinate
         * @param y the y coordinate
         */
        void set_cursor_position(uint8_t x, uint8_t y);
    };
}
