/**
 * @file display.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "display_font.h"

#include <string>
#include <esp_lcd_panel_ssd1306.h>

namespace Boboter::Libs::Display {
    namespace Config {
        static constexpr uint8_t I2C_ADDR = 0x3C;

        constexpr uint8_t WIDTH = 128;
        constexpr uint8_t HEIGHT = 64;
        constexpr uint8_t CHAR_WIDTH = 8;
        constexpr uint8_t CHAR_HEIGHT = 8;
    }

    class Display {
    private:
        static constexpr const char* TAG = "Libs::Display";

        uint8_t cursor_x, cursor_y;
        esp_lcd_panel_handle_t panel_handle;
        esp_lcd_panel_io_handle_t io_handle;

        uint8_t* display_buffer;

    public:
        explicit Display();
        ~Display();

        /**
         * @brief Clears the display
         *
         * @return (void)  
        */
        void clear();
        
        /**
         * @brief Writes Text to the display
         *
         * @return (void)  
         *
         * @param text The text to write
        */
        void write_text(std::string text);

        /**
         * @brief Sets the position of the cursor
         *
         * @return (void)  
         *
         * @param x The new x coordinate of the cursor
         * @param y The new y coordinate of the cursor
        */
        void set_cursor_pos(uint8_t x, uint8_t y);
    };
}