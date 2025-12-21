/**
 * @file gyro_helpers.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "gyro.hpp"

#include "helpers/i2c_utils.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Gyro {
    void Gyro::write_register(uint8_t reg, uint8_t data) {
        using namespace Config;
        using namespace Boboter::Helpers::I2C::Config;
        using namespace Boboter::Libs::Error;

        uint8_t buf[2] = { reg, data };
        WARN_CHECK(TAG, i2c_master_write_to_device(I2C_PORT, I2C_ADDRESS, buf, sizeof(buf), pdMS_TO_TICKS(100)));
    }

    void Gyro::read_registers(uint8_t reg, uint8_t *data, size_t len) {
        using namespace Config;
        using namespace Boboter::Helpers::I2C::Config;
        using namespace Boboter::Libs::Error;

        WARN_CHECK(TAG, i2c_master_write_read_device(I2C_PORT, I2C_ADDRESS, &reg, 1, data, len, pdMS_TO_TICKS(100)));
    }
}