// EXT_DISPLAY_CPP
// Boboter
// (C) MarioS271 2025

#include "io_shield.h"
#include <cstring>
#include "esp_log.h"
#include "delay.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "font8x8_basic.h"
#include "i2c_utils.h"

#define TAG "IO_SHIELD"

// Constructor
IOShield::IOShield() {
    esp_lcd_panel_io_i2c_config_t display_io_conf = {
        .dev_addr = DISPLAY_I2C_ADDR,
        .control_phase_bytes = 1,
        .dc_bit_offset = 6,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8
    };
    esp_lcd_new_panel_io_i2c(I2C_CONFIG::I2C_PORT, &display_io_conf, &io_handle);

    esp_lcd_panel_ssd1306_config_t display_vendor_conf = { .height = DISPLAY_HEIGHT };
    esp_lcd_panel_dev_config_t display_panel_conf = {
        .reset_gpio_num = GPIO_NUM_NC,
        .color_space = ESP_LCD_COLOR_SPACE_MONOCHROME,
        .bits_per_pixel = 1,
        .vendor_config = &display_vendor_conf
    };

    esp_lcd_new_panel_ssd1306(io_handle, &display_panel_conf, &panel);
    esp_lcd_panel_init(panel);
    esp_lcd_panel_disp_on_off(panel, true);
    esp_lcd_panel_mirror(panel, true, true);

    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    gpio_conf.mode = GPIO_MODE_INPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_conf);

    ESP_LOGI(TAG, "IOShield initialization completed");
}

// Clear Display
void IOShield::displayClear() {
    memset(displayBuffer, 0, sizeof(displayBuffer));
    esp_lcd_panel_draw_bitmap(panel, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, displayBuffer);

    cursorX = 0;
    cursorY = 0;

    ESP_LOGD(TAG, "Cleared Display");
}

// Set Cursor Position
void IOShield::displaySetCursorPos(uint8_t x, uint8_t y) {
    if (x >= DISPLAY_WIDTH) x = DISPLAY_WIDTH - CHAR_WIDTH;
    if (y >= DISPLAY_HEIGHT) y = DISPLAY_HEIGHT - CHAR_HEIGHT;

    cursorX = x;
    cursorY = y;

    ESP_LOGD(TAG, "Set Cursor Position on Display to X=%d, Y=%d", x, y);
}

// Write Text to Display
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
                if (pixel) {
                    uint16_t px = x + col;
                    uint16_t py = y + row;
                    size_t index = (py / 8) * DISPLAY_WIDTH + px;
                    displayBuffer[index] |= (1 << (py % 8));
                }
            }
        }

        x += CHAR_WIDTH;
    }

    esp_lcd_panel_draw_bitmap(panel, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, displayBuffer);

    cursorX = x;
    cursorY = y;

    ESP_LOGD(TAG, "Wrote Text \"%s\" to Display", text);
}




// Get Button State
bool IOShield::getButtonState() {
    bool state = gpio_get_level(BUTTON_PIN) == 0;

    if (state) { ESP_LOGD(TAG, "'getButtonState()' called while valid button press detected"); }
    return state;
}

// Get Button State (with built-in debounce)
bool IOShield::getDebouncedButtonState() {
    static uint32_t last_time = 0;
    static bool last_state = false;
    bool state = gpio_get_level(BUTTON_PIN) == 0;
    uint32_t now = xTaskGetTickCount();

    if (state != last_state && (now - last_time) > pdMS_TO_TICKS(BUTTON_DEBOUNCE)) {
        last_state = state;
        last_time = now;
        return state;
    }

    if (last_state) { ESP_LOGD(TAG, "'getDebouncedButtonState()' called while valid button press detected"); }
    return last_state;
}
