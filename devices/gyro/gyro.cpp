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
        device_handle(nullptr),
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

        write_register(REG_POWER_MANAGEMENT_1, 0x03);
        const uint8_t device_id = read_register(REG_WHO_AM_I);

        if (device_id != MPU6050_DEVICE_ID) {
            LOGE("MPU6050 not found! Expected 0x%02x, got 0x%02x", MPU6050_DEVICE_ID, device_id);
        }
        LOGI("MPU6050 with id 0x%02x found", device_id);

        LOGI("Initialized Device::Gyro");
    }

    void Gyro::calibrate() {
        // TODO: implement calibration
    }

    void Gyro::measure() {

    }
}
