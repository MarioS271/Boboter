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
        static constexpr uint8_t MPU6050_DEVICE_ID = 0x68;

        Robot& robot;
        i2c_master_dev_handle_t device_handle;
        vector3 gyro_values;
        vector3 accel_values;
        float temperature;

    private:
        /**
         * @brief Writes to a register
         *
         * @param register_address The address of the target register
         * @param value The value to write
         */
        void write_register(uint8_t register_address, uint8_t value) const;

        /**
         * @brief Reads a single byte from a register
         *
         * @return The value read from the register
         *
         * @param register_address The address of the register to read from
         */
        [[nodiscard]] uint8_t read_register(uint8_t register_address) const;

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
        [[nodiscard]] vector3 get_gyro_values() const { return gyro_values; }

        /**
         * @brief Retrieves the accel values from the last measurement
         *
         * @return A 3D vector struct of the x, y and z components of the accelerometer
         */
        [[nodiscard]] vector3 get_accel_values() const { return accel_values; }

        /**
         * @brief Retrieves the temperature value from the last measurement
         *
         * @return A floating point number of the temperature in °C (celsius)
         */
        [[nodiscard]] float get_temperature() const { return temperature; }
    };
}