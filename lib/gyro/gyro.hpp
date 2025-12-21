/**
 * @file gyro.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>
#include <driver/i2c.h>

namespace Boboter::Libs::Gyro {
    namespace Config {
        constexpr uint8_t I2C_ADDRESS = 0x68;
        constexpr uint16_t CALIBRATION_SAMPLES = 500;

        constexpr float ACCEL_SCALE = 16384.0f;
        constexpr float GYRO_SCALE = 131.0f;
        constexpr float TEMP_SENSITIVITY = 340.0f;
        constexpr float TEMP_OFFSET = 36.53f;
    }

    namespace Constants {
        constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
        constexpr uint8_t REG_ACCEL_XOUT_H = 0x3B;
        constexpr uint8_t REG_GYRO_XOUT_H = 0x43;
        constexpr uint8_t REG_WHO_AM_I = 0x75;
    }

    class Gyro {
    private:
        static constexpr const char* TAG = "Libs::Gyro";
    
        float offset_gyro_x, offset_gyro_y, offset_gyro_z;
        float accel_x, accel_y, accel_z;
        float gyro_x, gyro_y, gyro_z;
        float temperature;

        void write_register(uint8_t reg, uint8_t data);
        void read_registers(uint8_t reg, uint8_t *data, size_t len);
    
    public:
        explicit Gyro();
        ~Gyro() = default;
    
        /**
         * @brief Calibrates the gyro
         *
         * @return (void)  
        */
        void calibrate();

        /**
         * @brief Reads the values from the gyro and stores them in the class
         *
         * @return (void)  
        */
        void read();

        /**
         * @brief Gets the current x axis acceleration value
         *
         * @return (float) The current x axis acceleration
        */
        float get_accel_x() const { return accel_x; }

        /**
         * @brief Gets the current y axis acceleration value
         *
         * @return (float) The current y axis acceleration
        */
        float get_accel_y() const { return accel_y; }

        /**
         * @brief Gets the current z axis acceleration value
         *
         * @return (float) The current z axis acceleration
        */
        float get_accel_z() const { return accel_z; }

        /**
         * @brief Gets the current x axis gyroscope value
         *
         * @return (float) The current x axis rotation speed
        */
        float get_gyro_x() const { return gyro_x; }

        /**
         * @brief Gets the current y axis gyroscope value
         *
         * @return (float) The current Y axis rotation speed
        */
        float get_gyro_y() const { return gyro_y; }

        /**
         * @brief Gets the current z axis gyroscope value
         *
         * @return (float) The current z axis rotation speed
        */
        float get_gyro_z() const { return gyro_z; }

        /**
         * @brief Gets the current temperature value
         *
         * @return (float) The current temperature (in degrees celsius)
        */
        float get_temperature() const { return temperature; }
    };
}