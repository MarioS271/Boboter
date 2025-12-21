/**
 * @file magnetometer.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "magnetometer.hpp"

#include <cmath>
#include <algorithm>
#include "helpers/delay.hpp"

namespace Boboter::Libs::Magnetometer {
    Magnetometer::Magnetometer()
    : offset_mag_x(0.0f), offset_mag_y(0.0f), offset_mag_z(0.0f),
      mag_x(0.0f), mag_y(0.0f), mag_z(0.0f),
      heading(0.0f)
    {
        using namespace Constants;
        using Boboter::Helpers::delay;

        write_register(REG_RESET, 0x01);
        delay(10);
        write_register(REG_CONF_1, 0x1D);
    }

    void Magnetometer::calibrate() {
        using namespace Config;
        using Boboter::Helpers::delay;
        
        offset_mag_x = 0.0f;
        offset_mag_y = 0.0f;
        offset_mag_z = 0.0f;

        float min_x = 9999, max_x = -9999;
        float min_y = 9999, max_y = -9999;
        float min_z = 9999, max_z = -9999;

        for (uint16_t i = 0; i < CALIBRATION_SAMPLES; i++) {
            read();
            
            min_x = std::min(min_x, mag_x);
            max_x = std::max(max_x, mag_x);

            min_y = std::min(min_y, mag_y);
            max_y = std::max(max_y, mag_y);

            min_z = std::min(min_z, mag_z);
            max_z = std::max(max_z, mag_z);

            delay(10);
        }

        offset_mag_x = (max_x + min_x) / 2.0f;
        offset_mag_y = (max_y + min_y) / 2.0f;
        offset_mag_z = (max_z + min_z) / 2.0f;
    }

    void Magnetometer::read() {
        using namespace Config;
        using namespace Constants;

        uint8_t buffer[6];
        read_registers(REG_DATA_X_LSB, buffer, 6);

        auto combine = [](uint8_t h, uint8_t l) {
            return static_cast<int16_t>((h << 8) | l);
        };

        int16_t raw_x = combine(buffer[1], buffer[0]);
        int16_t raw_y = combine(buffer[3], buffer[2]);
        int16_t raw_z = combine(buffer[5], buffer[4]);

        mag_x = (static_cast<float>(raw_x) / MAG_SCALE) - offset_mag_x;
        mag_y = (static_cast<float>(raw_y) / MAG_SCALE) - offset_mag_y;
        mag_z = (static_cast<float>(raw_z) / MAG_SCALE) - offset_mag_z;

        heading = std::atan2(mag_y, mag_x) * 180.0f / M_PI;

        if (heading < 0.0f)
            heading += 360.0f;
    }
}