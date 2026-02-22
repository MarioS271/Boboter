/**
 * @file gyro.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "gyro.h"

#include <esp_lcd_io_i2c.h>

#include "lib/logger/logger.h"
#include "include/robot.h"

namespace Device {
    Gyro::Gyro(Robot& robot) :
        robot(robot),
        gyro_values{},
        accel_values{},
        temperature(-1.0f)
    {
        LOGD("Constructor called");
    }

    Gyro::~Gyro() {
        LOGD("Destructor called");
    }

    void Gyro::initialize() {
        device_handle = robot.i2c.add_device(
            HAL::I2C::device_config_t{
                .address = I2C_ADDRESS,
                .clock_speed = 400'000,
                .address_bit_length = I2C_ADDR_BIT_LEN_7
            }
        );

        // uint8_t read_buffer;
        // uint8_t write_buffer = { REG_POWER_MANAGEMENT_1, 0x03 };
        // robot.i2c.write(device_handle, &write_buffer, sizeof(write_buffer));
        //
        // write_buffer = { REG_WHO_AM_I };
        // robot.i2c.write_read(
        //     device_handle,
        //     &write_buffer,
        //     sizeof(write_buffer),
        //     &read_buffer,
        //     sizeof(read_buffer)
        // );
        //
        // if (read_buffer != I2C_ADDRESS) {
        //     LOGE("Device at 0x%02x is not an MPU6050", read_buffer);
        //     halt_execution();
        // }

        LOGI("Initialized Device::Gyro");
    }

    void Gyro::calibrate() {
        // TODO: implement calibration
    }

    void Gyro::measure() {

    }
}
