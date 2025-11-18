// GYRO_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"

class Gyro {
private:
    static constexpr char* TAG = "GYRO";
    
    static constexpr i2c_port_t I2C_PORT = I2C_NUM_0;
    static constexpr uint8_t I2C_ADDRESS = 0x68;

    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;

    float pitch_offset, yaw_offset, roll_offset;
    float pitch_total, yaw_total, roll_total;

    float yaw_bias;

    uint64_t last_update_ms;
    bool calibrating;

    void writeRegister(uint8_t reg, uint8_t data);
    void readRegisters(uint8_t reg, uint8_t *data, size_t len);

    uint64_t millis() const { return xTaskGetTickCount() * portTICK_PERIOD_MS; }

public:
    Gyro();
    ~Gyro() = default;

    void calibrate();
    bool isCalibrating() const { return calibrating; }

    void update(bool ignore_is_calibrating = false);

    float getPitch() const { return pitch_total; }
    float getYaw() const { return yaw_total; }
    float getRoll() const { return roll_total; }
};