/**
 * @file gyro.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "gyro.hpp"

#include "helpers/delay.hpp"
#include "lib/logger/logger.hpp"

namespace Boboter::Libs::Gyro {
    Gyro::Gyro()
    : offset_gyro_x(0.0f), offset_gyro_y(0.0f), offset_gyro_z(0.0f),
      accel_x(0.0f), accel_y(0.0f), accel_z(0.0f),
      gyro_x(0.0f), gyro_y(0.0f), gyro_z(0.0f),
      temperature(0.0f)
    {
        using namespace Config;
        using namespace Constants;
        using namespace Boboter::Libs::Logger;

        write_register(REG_PWR_MGMT_1, 0x03);
        
        uint8_t id = 0;
        read_registers(REG_WHO_AM_I, &id, 1);

        if (id != 0x68) {
            LOGE(TAG, "MPU6050 not found, instead found device with id 0x%02x", id);
            abort();
        }

        LOGI(TAG, "Initialized Gyro");

        calibrate();
    }
    
    void Gyro::calibrate() {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using Boboter::Helpers::delay;

        LOGI(TAG, "Calibrating... Keep robot still!");

        offset_gyro_x = 0.0f;
        offset_gyro_y = 0.0f;
        offset_gyro_z = 0.0f;
        
        float sum_x = 0, sum_y = 0, sum_z = 0;

        for (int i = 0; i < CALIBRATION_SAMPLES; ++i) {
            read();
            sum_x += gyro_x;
            sum_y += gyro_y;
            sum_z += gyro_z;
            delay(2);
        }

        offset_gyro_x = sum_x / CALIBRATION_SAMPLES;
        offset_gyro_y = sum_y / CALIBRATION_SAMPLES;
        offset_gyro_z = sum_z / CALIBRATION_SAMPLES;

        LOGI(TAG, "Calibration done. Offsets: X:%.2f Y:%.2f Z:%.2f", 
            offset_gyro_x, offset_gyro_y, offset_gyro_z);
    }

    void Gyro::read() {
        using namespace Config;
        using namespace Constants;
        
        uint8_t data[14];
        read_registers(REG_ACCEL_XOUT_H, data, 14);

        auto combine = [](uint8_t h, uint8_t l) {
            return static_cast<int16_t>((h << 8) | l);
        };

        accel_x = combine(data[0], data[1]) / ACCEL_SCALE;
        accel_y = combine(data[2], data[3]) / ACCEL_SCALE;
        accel_z = combine(data[4], data[5]) / ACCEL_SCALE;

        int16_t raw_temperature = combine(data[6], data[7]);
        temperature = (static_cast<float>(raw_temperature) / TEMP_SENSITIVITY) + TEMP_OFFSET;

        gyro_x = (combine(data[8], data[9]) / GYRO_SCALE) - offset_gyro_x;
        gyro_y = (combine(data[10], data[11]) / GYRO_SCALE) - offset_gyro_y;
        gyro_z = (combine(data[12], data[13]) / GYRO_SCALE) - offset_gyro_z;
    }
}