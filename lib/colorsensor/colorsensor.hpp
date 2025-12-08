/**
 * @file colorsensor.hpp
 * @authors MarioS271
 */

#pragma once

#include "colorsensor_types.hpp"

#include <driver/gpio.h>
#include <driver/i2c.h>
#include "rgb_color.hpp"

class Colorsensor
{
private:
    static constexpr const char* TAG = "Colorsensor";

    static constexpr uint8_t SENSOR_ADDR = 0x29;
    static constexpr uint8_t CMD_BIT = 0x80;

    static constexpr uint8_t REG_ENABLE  = 0x00;
    static constexpr uint8_t REG_ATIME   = 0x01;
    static constexpr uint8_t REG_CONTROL = 0x0F;
    static constexpr uint8_t REG_CDATAL  = 0x14;

    static constexpr uint8_t ENABLE_PON = 0x01;
    static constexpr uint8_t ENABLE_AEN = 0x02;

    i2c_port_t i2c_port;
    IntegrationTime it;
    Gain gain;
    bool enabled;
    
    esp_err_t writeRegister(uint8_t reg, uint8_t value);
    esp_err_t readRegister(uint8_t reg, uint8_t &value);
    esp_err_t readRegisters(uint8_t reg, uint8_t *buffer, size_t len);

public:
    Colorsensor();
    ~Colorsensor() = default;

    void enable();
    void disable();

    rgb_color_t readRGB();
    rgb_color_t readRGBOnce();
};