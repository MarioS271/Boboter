/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/constants.h"
#include "include/flags.h"
#include "include/robot.h"
#include "helpers/delay.h"
#include "helpers/map.h"
#include "lib/logger/logger.h"

void send_command(const i2c_master_dev_handle_t&, uint8_t);

extern "C" void app_main() {
    using namespace Constants;

    const Robot& robot = Robot::get_instance();

    robot.adc.configure(
        ADC::controller_config_t{
            .attenuation = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_12,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
            .clock_source = ADC_RTC_CLK_SRC_DEFAULT
        }
    );
    robot.adc.add(ADC_CHANNEL_3);

    robot.i2c.configure(
        I2C::bus_config_t{
            .port = I2C_NUM_0,
            .sda_pin = GPIO_NUM_21,
            .scl_pin = GPIO_NUM_22
        }
    );
    robot.i2c.scan_for_devices();

    i2c_master_dev_handle_t display = robot.i2c.add_device(
        I2C::device_config_t{
            .address = 0x3C,
            .clock_speed = 400'000,
            .address_bit_length = I2C_ADDR_BIT_LEN_7
        }
    );

    send_command(display, 0xAE); // Display OFF
    send_command(display, 0x8D); // Charge Pump
    send_command(display, 0x14); // Enable Charge Pump
    send_command(display, 0xAF); // Display ON
    send_command(display, 0x20); // Memory Addressing Mode
    send_command(display, 0x00); // Horizontal Addressing

    uint8_t full_buffer[1025];
    full_buffer[0] = 0x40;

    for (int i = 1; i <= 1024; i++) {
        const bool even_column = (i - 1) % 2 == 0;
        const bool even_page = (i - 1) / 128 % 2 == 0;

        if (even_page) {
            full_buffer[i] = even_column ? 0xAA : 0x55;
        } else {
            full_buffer[i] = even_column ? 0x55 : 0xAA;
        }
    }
    robot.i2c.write(display, full_buffer, 1025);

    while (true) {
        uint16_t voltage = robot.adc.read_millivolts(ADC_CHANNEL_3, 100);
        voltage = voltage * 147 / 100;

        auto percentage = static_cast<int8_t>(map_value(voltage, 3300, 4200, 0, 100));

        if (percentage > 100)
            percentage = 100;

        LOGI("Battery Voltage / Percentage: %d mV / %d%%", voltage, percentage);
        delay(500);
    }
}

void send_command(const i2c_master_dev_handle_t& device, const uint8_t command) {
    const uint8_t buffer[] = {0x00, command};
    Robot::get_instance().i2c.write(device, buffer, 2);
}