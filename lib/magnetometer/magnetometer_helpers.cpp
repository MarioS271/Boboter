/**
 * @file magnetometer_helpers.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "magnetometer.hpp"

#include "helpers/i2c_utils.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Magnetometer {
    void Magnetometer::write_register(uint8_t reg, uint8_t data) {
        using namespace Config;
        using namespace Boboter::Helpers::I2C::Config;
        using namespace Boboter::Libs::Error;

        uint8_t write_buf[2] = {reg, data};
        i2c_master_write_to_device(I2C_PORT, I2C_ADDRESS, write_buf, 2, pdMS_TO_TICKS(100));
    }

    void Magnetometer::read_registers(uint8_t reg, uint8_t *data, size_t len) {
        using namespace Config;
        using namespace Boboter::Helpers::I2C::Config;
        using namespace Boboter::Libs::Error;

        WARN_CHECK(TAG, i2c_master_write_read_device(I2C_PORT, I2C_ADDRESS, &reg, 1, data, len, pdMS_TO_TICKS(100)));
    }
}