/**
 * @file gyro.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <driver/i2c_types.h>
#include "types/vector3.h"
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Gyro {
    private:
        static constexpr const char* TAG = "Device::Gyro";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_GYRO;

        static constexpr uint8_t I2C_ADDRESS = 0x68;

        Robot& robot;
        i2c_master_dev_handle_t device_handle;
        vector3 gyro_values;
        vector3 accel_values;
        float temperature;

    private:
        #include "gyro_registers.inc"

    public:
        explicit Gyro(Robot& robot);
        ~Gyro();

        /**
         * @brief Sets up the necessary I2C channel
         */
        void initialize();

        /**
         * @brief Calibrates the gyro
         */
        void calibrate();

        /**
         * @brief Reads the current values from the gyro
         */
        void measure();

        /**
         * @brief Retrieves the gyro values from the last measurement
         *
         * @return A 3D vector struct of the x, y and z components of the gyroscope
         */
        vector3 get_gyro_values() const { return gyro_values; }

        /**
         * @brief Retrieves the accel values from the last measurement
         *
         * @return A 3D vector struct of the x, y and z components of the accelerometer
         */
        vector3 get_accel_values() const { return accel_values; }

        /**
         * @brief Retrieves the temperature value from the last measurement
         *
         * @return A floating point number of the temperature in °C (celsius)
         */
        float get_temperature() const { return temperature; }
    };
}