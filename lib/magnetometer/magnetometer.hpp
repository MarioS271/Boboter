/**
 * @file magnetometer.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>
#include <driver/i2c.h>

namespace Boboter::Libs::Magnetometer {
    namespace Config {
        constexpr uint8_t I2C_ADDRESS = 0x0D;
        constexpr uint16_t CALIBRATION_SAMPLES = 500;
        constexpr float MAG_SCALE = 3000.0f;
    }

    namespace Constants {
        constexpr uint8_t REG_DATA_X_LSB = 0x00;
        constexpr uint8_t REG_CONF_1 = 0x09;
        constexpr uint8_t REG_CONF_2 = 0x0A;
        constexpr uint8_t REG_RESET = 0x0B;
    }

    class Magnetometer {
    private:
        static constexpr const char* TAG = "Libs::Magnetometer";

        float offset_mag_x, offset_mag_y, offset_mag_z;
        float mag_x, mag_y, mag_z;
        float heading;

        void write_register(uint8_t reg, uint8_t data);
        void read_registers(uint8_t reg, uint8_t *data, size_t len);

    public:
        explicit Magnetometer();
        ~Magnetometer() = default;

        /**
         * @brief Calibrates the magnetometer
         *
         * @return (void)  
        */
        void calibrate();

        /**
         * @brief Reads the values from the magnetometer and stores them in the class
         *
         * @return (void)  
        */
        void read();

        /**
         * @brief Gets the current x axis magnetic value
         *
         * @return (float) The current x axis magnetic value
        */
        float get_mag_x() const { return mag_x; }

        /**
         * @brief Gets the current y axis magnetic value
         *
         * @return (float) The current y axis magnetic value
        */
        float get_mag_y() const { return mag_y; }

        /**
         * @brief Gets the current z axis magnetic value
         *
         * @return (float) The current z axis magnetic value
        */
        float get_mag_z() const { return mag_z; }

        /**
         * @brief Gets the current calculated heading
         *
         * @return (float) The current heading
        */
        float get_heading() const { return heading; }
    };
}