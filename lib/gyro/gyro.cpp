// GYRO_CPP
// Boboter
// (C) MarioS271 2025


#include "gyro.h"
#include "esp_log.h"
#include "delay.h"
#include <cmath>

#define TAG "GYRO"

// MPU6050 full-scale gyro = ±250 deg/s → 131 LSB/(°/s)
constexpr float GYRO_SCALE = 131.0f;

// Constructor
Gyro::Gyro()
: accel_x(0), accel_y(0), accel_z(0),
  gyro_x(0), gyro_y(0), gyro_z(0),
  pitch_offset(0), yaw_offset(0), roll_offset(0),
  pitch_total(0), yaw_total(0), roll_total(0),
  last_update_ms(0),
  calibrating(false)
{
    writeRegister(0x6B, 0x00); // wake MPU6050
    delay(100);
    last_update_ms = millis();
    ESP_LOGI(TAG, "Initialized MPU6050 Gyro");
}

// Private helpers
void Gyro::writeRegister(uint8_t reg, uint8_t data) {
    uint8_t buf[2] = { reg, data };
    i2c_master_write_to_device(I2C_PORT, I2C_ADDRESS, buf, 2, pdMS_TO_TICKS(100));
}

void Gyro::readRegisters(uint8_t reg, uint8_t *data, size_t len) {
    i2c_master_write_read_device(I2C_PORT, I2C_ADDRESS, &reg, 1, data, len, pdMS_TO_TICKS(100));
}

// Multi-sample calibration
void Gyro::calibrate() {
    calibrating = true;

    const int samples = 500;
    float sum_x = 0, sum_y = 0, sum_z = 0;

    for (int i = 0; i < samples; i++) {
        uint8_t raw[14];
        readRegisters(0x3B, raw, 14);

        // Convert raw gyro to deg/s
        float gx = static_cast<int16_t>((raw[8] << 8) | raw[9]) / GYRO_SCALE;
        float gy = static_cast<int16_t>((raw[10] << 8) | raw[11]) / GYRO_SCALE;
        float gz = static_cast<int16_t>((raw[12] << 8) | raw[13]) / GYRO_SCALE;

        sum_x += gx;
        sum_y += gy;
        sum_z += gz;

        delay(5);
    }

    roll_offset  = sum_x / samples;
    pitch_offset = sum_y / samples;
    yaw_offset   = sum_z / samples;

    roll_total = pitch_total = yaw_total = 0;
    calibrating = false;

    ESP_LOGI(TAG, "Gyro calibrated: roll=%f pitch=%f yaw=%f", roll_offset, pitch_offset, yaw_offset);
}

// Update function with complementary filter
void Gyro::update(bool ignore_is_calibrating) {
    if (calibrating && !ignore_is_calibrating) return;

    uint8_t raw[14];
    readRegisters(0x3B, raw, 14);

    // Convert raw accelerometer data to g
    accel_x = static_cast<int16_t>((raw[0] << 8) | raw[1]) / 16384.0f;
    accel_y = static_cast<int16_t>((raw[2] << 8) | raw[3]) / 16384.0f;
    accel_z = static_cast<int16_t>((raw[4] << 8) | raw[5]) / 16384.0f;

    // Convert raw gyro data to deg/s
    gyro_x = static_cast<int16_t>((raw[8] << 8) | raw[9]) / GYRO_SCALE;
    gyro_y = static_cast<int16_t>((raw[10] << 8) | raw[11]) / GYRO_SCALE;
    gyro_z = static_cast<int16_t>((raw[12] << 8) | raw[13]) / GYRO_SCALE;

    // Compute time delta
    uint64_t now = millis();
    float dt = (now - last_update_ms) / 1000.0f;
    last_update_ms = now;

    // Accelerometer-based angles
    float pitch_acc = atan2f(accel_y, sqrtf(accel_x*accel_x + accel_z*accel_z)) * 180.0f / M_PI;
    float roll_acc  = atan2f(-accel_x, accel_z) * 180.0f / M_PI;

    // Complementary filter: alpha lower for faster bias correction
    constexpr float alpha = 0.96f;
    roll_total  = alpha * (roll_total + (gyro_x - roll_offset) * dt) + (1.0f - alpha) * roll_acc;
    pitch_total = alpha * (pitch_total + (gyro_y - pitch_offset) * dt) + (1.0f - alpha) * pitch_acc;

    // Yaw: integrate gyro with simple high-pass filter
    constexpr float yaw_filter = 0.98f; // lower = stronger bias removal
    float yaw_rate = gyro_z - yaw_offset;
    yaw_total = yaw_filter * (yaw_total + yaw_rate * dt);

    ESP_LOGD(TAG,
        "Accel: x=%f y=%f z=%f | Gyro: x=%f y=%f z=%f | Roll=%f Pitch=%f Yaw=%f",
        accel_x, accel_y, accel_z,
        gyro_x - roll_offset, gyro_y - pitch_offset, gyro_z - yaw_offset,
        roll_total, pitch_total, yaw_total
    );
}
