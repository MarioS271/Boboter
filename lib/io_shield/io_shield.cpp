// EXT_DISPLAY_CPP
// Boboter
// (C) MarioS271 2025

#include "io_shield.h"
#include <cstring>
#include "esp_log.h"
#include "ssd1306/font8x8_basic.h"

#define TAG "IO_SHIELD"

// Constructor
IOShield::IOShield() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {.clk_speed = 400000},
        .clk_flags = 0,
    };
    i2c_param_config(DISPLAY_I2C_PORT, &conf);
    i2c_driver_install(DISPLAY_I2C_PORT, conf.mode, 0, 0, 0);

    ssd1306_init(&dev, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ssd1306_clear_screen(&dev, false);
    ssd1306_show_buffer(&dev);

    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    gpio_conf.mode = GPIO_MODE_INPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_conf);

    cursorX = 0;
    cursorY = 0;

    ESP_LOGI(TAG, "IOShield initialization completed");
}

// Clear Display
void IOShield::displayClear() {
    ssd1306_clear_screen(&dev, false);
    ssd1306_show_buffer(&dev);

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
    int page = cursorY / CHAR_HEIGHT;
    int text_len = strlen(text);
    if (text_len > 16) text_len = 16; // Max 16 Zeichen pro Page

    ssd1306_display_text(&dev, page, text, text_len, false);

    cursorY += CHAR_HEIGHT;
    cursorX = 0;
    
    ssd1306_show_buffer(&dev);

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
