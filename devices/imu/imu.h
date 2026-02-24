/**
 * @file imu.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <driver/i2c_types.h>
#include "types/vector3.h"
#include "types/quaternion.h"
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Imu {
    private:
        static constexpr const char* TAG = "Device::Imu";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_IMU;

        static constexpr uint8_t I2C_ADDRESS = 0x68;
        static constexpr uint32_t I2C_CLOCK_SPEED = 100'000;
        static constexpr uint8_t MPU6050_DEVICE_ID = 0x68;
        static constexpr uint8_t FIFO_BUFFER_SIZE = 42;

        Robot& robot;
        i2c_master_dev_handle_t device_handle;
        quaternion quaternion_values;
        vector3 gyro_values;

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
        #include "imu_registers.inc.h"
        #include "imu_dmp_firmware.inc.h"

    public:
        explicit Imu(Robot& robot);
        ~Imu();

        /**
         * @brief Sets up the necessary I2C channel
         */
        void initialize();

        /**
         * @brief Reads the current FIFO buffer from the IMUs DMP (digital motion processor)
         */
        void read_fifo_buffer();

        /**
         * @brief Retrieves the quaternion from the last measurement
         *
         * @return The last read quaternion
         */
        [[nodiscard]] quaternion get_quaternion() const { return quaternion_values; }

        /**
         * @brief Retrieves the gyro values from the last measurement
         *
         * @return A 3D vector struct of the x, y and z components of the gyroscope
         */
        [[nodiscard]] vector3 get_gyro_values() const { return gyro_values; }
    };
}