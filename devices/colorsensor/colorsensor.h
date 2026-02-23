/**
 * @file colorsensor.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <driver/i2c_types.h>
#include "types/rgb_color.h"
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Colorsensor {
    private:
        static constexpr const char* TAG = "Device::Colorsensor";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_COLORSENSOR;

        static constexpr uint8_t I2C_ADDRESS = 0x29;
        static constexpr uint8_t TCS34725_DEVICE_ID = 0x44;

        Robot& robot;
        i2c_master_dev_handle_t device_handle;
        rgb_color_t color;

    private:
        /**
         * @brief Writes to a register with the command start byte appended as a prefix
         *
         * @param register_address The address of the target register
         * @param value The value to write
         */
        void write_register(uint8_t register_address, uint8_t value) const;

        /**
         * @brief Reads a single byte from a register.
         *
         * @return The value read from the register.
         *
         * @param register_address The address of the register to read from
         */
        [[nodiscard]] uint8_t read_register(uint8_t register_address) const;

    private:
        #include "colorsensor_registers.inc"

    public:
        explicit Colorsensor(Robot& robot);
        ~Colorsensor();

        /**
         * @brief Sets up the necessary I2C channel
         */
        void initialize();

        /**
         * @brief Measures the current sensor values and computes them into color and brightness
         */
        void measure();

        /**
         * @brief Gets the color seen on last measurement
         * @note The color of the return is of the type @c rgb_color_t, but is always picked from @c predef_color.h
         *
         * @return The color formatted as a @c rgb_color_t
         */
        [[nodiscard]] rgb_color_t get_color() const { return color; }
    };
}