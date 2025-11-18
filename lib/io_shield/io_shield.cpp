// EXT_DISPLAY_CPP
// Boboter
// (C) MarioS271 2025

#include "io_shield.h"

#include <cstring>
#include "logger.h"
#include "delay.h"
#include "i2c_utils.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

#define TAG "IO_SHIELD"

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

    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);

    LOGI(TAG, "IOShield initialization completed");
}

IOShield::~IOShield() {
    if (panel) {
        esp_lcd_panel_del(panel);
        panel = nullptr;
    }

    if (io_handle) {
        esp_lcd_panel_io_del(io_handle);
        io_handle = nullptr;
    }
}