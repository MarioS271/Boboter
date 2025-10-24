// GYRO_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/i2c.h"

class Gyro {
private:
    static constexpr uint8_t address = 0x0D;
    static constexpr i2c_port_t i2c_port = I2C_NUM_0;

    int16_t x;
    int16_t y;
    int16_t z;

    void writeRegister(uint8_t reg, uint8_t value);
    void readRegisters(uint8_t reg, uint8_t *data, size_t length);

public:
    explicit Gyro();

    void read();

    float getPitch();
    float getYaw();
    float getRoll();
};