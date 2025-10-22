// GYRO_CPP
// Boboter
// (C) MarioS271 2025

#include "gyro.h"
#include <cmath>
#include "esp_log.h"

static const char* TAG = "GYRO";

Gyro::Gyro() : x(0), y(0), z(0) {}

void Gyro::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    i2c_master_write_to_device(i2c_port, address, data, 2, pdMS_TO_TICKS(100));
}

void Gyro::readRegisters(uint8_t reg, uint8_t *data, size_t length) {
    i2c_master_write_read_device(i2c_port, address, &reg, 1, data, length, pdMS_TO_TICKS(100));
}

void Gyro::read() {
    uint8_t data[6];
    readRegisters(0x00, data, 6);
    x = (int16_t)(data[1] << 8 | data[0]);
    y = (int16_t)(data[3] << 8 | data[2]);
    z = (int16_t)(data[5] << 8 | data[4]);
}

float Gyro::getYaw() {
    float heading = atan2((double)y, (double)x) * 180.0 / M_PI;
    if (heading < 0) heading += 360.0;
    return heading;
}

float Gyro::getPitch() {
    return atan2(-x, sqrt(y * y + z * z)) * 180.0 / M_PI;
}

float Gyro::getRoll() {
    return atan2(y, z) * 180.0 / M_PI;
}
