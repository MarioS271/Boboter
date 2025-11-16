// I2C_UTILS_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "logger.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

namespace I2C_CONFIG {
    constexpr gpio_num_t SDA_GPIO = GPIO_NUM_21;
    constexpr gpio_num_t SCL_GPIO = GPIO_NUM_22;

    constexpr i2c_port_t I2C_PORT = I2C_NUM_0;
    constexpr uint32_t I2C_FREQ = 100000; // Hz
}

inline void init_i2c() {
    using namespace I2C_CONFIG;

    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = { .clk_speed = I2C_FREQ },
        .clk_flags = 0
    };
    i2c_param_config(I2C_PORT, &i2c_conf);
    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
}

inline void scan_i2c_addresses(const char* TAG) {
    using namespace I2C_CONFIG;

    LOGI(TAG, "Scanning I2C bus for device addresses");

    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);

        esp_err_t err = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(50));
        i2c_cmd_link_delete(cmd);

        if (err == ESP_OK) {
            LOGI(TAG, "(i2c_scan) Device found at 0x%02x", addr);
        }
    }
}