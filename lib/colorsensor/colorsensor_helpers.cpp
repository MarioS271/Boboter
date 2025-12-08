/**
 * @file colorsensor_helpers.cpp
 * @authors MarioS271
 */

#include "colorsensor.hpp"

esp_err_t Colorsensor::writeRegister(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = { static_cast<uint8_t>(CMD_BIT | reg), value };
    return i2c_master_write_to_device(i2c_port, SENSOR_ADDR, data, 2, pdMS_TO_TICKS(50));
}

esp_err_t Colorsensor::readRegister(uint8_t reg, uint8_t &value)
{
    uint8_t cmd = CMD_BIT | reg;
    return i2c_master_write_read_device(i2c_port, SENSOR_ADDR, &cmd, 1, &value, 1, pdMS_TO_TICKS(50));
}

esp_err_t Colorsensor::readRegisters(uint8_t reg, uint8_t *buffer, size_t len) 
{
    uint8_t cmd = CMD_BIT | reg;
    return i2c_master_write_read_device(i2c_port, SENSOR_ADDR, &cmd, 1, buffer, len, pdMS_TO_TICKS(50));
}