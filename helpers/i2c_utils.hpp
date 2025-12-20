/**
 * @file i2c_utils.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <driver/gpio.h>
#include <driver/i2c.h>
#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"
 
namespace Boboter::Helpers::I2C {
    namespace Config {
        constexpr gpio_num_t SDA_GPIO = GPIO_NUM_21;
        constexpr gpio_num_t SCL_GPIO = GPIO_NUM_22;
        
        constexpr i2c_port_t I2C_PORT = I2C_NUM_0;
        constexpr uint32_t I2C_FREQ = 100000; // Hz
    }
    
    namespace Internal {
        constexpr const char* TAG = "Helper::I2C";
    }

    /**
     * @brief Initializes and configures I2C
     *
     * @return (void)  
    */
    inline void init() {
        using namespace Internal;
        using namespace Config;
        using namespace Boboter::Libs::Error;

        i2c_config_t i2c_conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_GPIO,
            .scl_io_num = SCL_GPIO,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master = { .clk_speed = I2C_FREQ },
            .clk_flags = 0
        };
        ERROR_CHECK(TAG, i2c_param_config(I2C_PORT, &i2c_conf));
        ERROR_CHECK(TAG, i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));
    }

    /**
     * @brief Scans the I2C bus for connected devices and their addresses
     *
     * @return (void)  
    */
    inline void scan_adresses() {
        using namespace Internal;
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        LOGI(TAG, "Scanning I2C bus for device addresses");

        for (uint8_t addr = 1; addr < 127; addr++) {
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();

            ERROR_CHECK(TAG, i2c_master_start(cmd));
            ERROR_CHECK(TAG, i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true));
            ERROR_CHECK(TAG, i2c_master_stop(cmd));

            esp_err_t res = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(50));
            i2c_cmd_link_delete(cmd);

            if (res == ESP_OK)
                LOGI(TAG, " -> Device found at 0x%02x", addr);
        }
    }
}