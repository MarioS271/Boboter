/**
 * @file imu_helpers.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "imu.h"

#include "include/robot.h"

namespace Device {
    uint8_t Imu::read_register(const uint8_t register_address) const {
        uint8_t value = 0;
        robot.i2c.write_read(device_handle, &register_address, 1, &value, 1);
        return value;
    }

    void Imu::write_register(const uint8_t register_address, const uint8_t value) const {
        const uint8_t buffer[2] = { static_cast<uint8_t>(register_address), value };
        robot.i2c.write(device_handle, buffer, sizeof(buffer), 10);
    }
}
