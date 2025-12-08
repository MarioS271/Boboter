/**
 * @file colorsensor.cpp
 * @authors MarioS271
 */

#include "colorsensor.hpp"

#include <freertos/FreeRTOS.h>
#include <driver/i2c.h>
#include "delay.hpp"
#include "i2c_utils.hpp"
#include "logger.hpp"
#include "error.hpp"

Colorsensor::Colorsensor()
: i2c_port(I2C_Config::I2C_PORT),
  it(IntegrationTime::MS_50),
  gain(Gain::GAIN_4X),
  enabled(false)
{
    uint8_t id = 0;
    ERROR_CHECK(TAG, readRegister(0x12, id));
    if (id != 0x44 && id != 0x4D)
    {
        LOGE(TAG, "Invalid TCS34725 ID: 0x%02X", id);
        abort();
    }
    
    ERROR_CHECK(TAG, writeRegister(REG_ATIME, static_cast<uint8_t>(it)));
    ERROR_CHECK(TAG, writeRegister(REG_CONTROL, static_cast<uint8_t>(gain)));

    LOGI(TAG, "Initialized Color Sensor");
}

void Colorsensor::enable()
{
    if (!enabled) {
        ERROR_CHECK(TAG, writeRegister(REG_ENABLE, ENABLE_PON));
        delay(4);
        ERROR_CHECK(TAG, writeRegister(REG_ENABLE, ENABLE_PON | ENABLE_AEN));

        uint16_t delay_ms = 0;
        switch (it)
        {
            case IntegrationTime::MS_2_4: delay_ms = 3; break;
            case IntegrationTime::MS_24:  delay_ms = 24; break;
            case IntegrationTime::MS_50:  delay_ms = 50; break;
            case IntegrationTime::MS_101: delay_ms = 101; break;
            case IntegrationTime::MS_154: delay_ms = 154; break;
            case IntegrationTime::MS_700: delay_ms = 700; break;
        }
        delay(delay_ms + 1);

        enabled = true;
    }
}

void Colorsensor::disable()
{
    if (enabled)
    {
        ERROR_CHECK(TAG, writeRegister(REG_ENABLE, 0x00));
        enabled = false;
    }
}

rgb_color_t Colorsensor::readRGB()
{
    if (!enabled)
        enable();

    uint16_t delay_ms = ((256 - static_cast<uint8_t>(it)) * 12) / 5 + 1;
    vTaskDelay(pdMS_TO_TICKS(delay_ms));

    uint8_t raw[8];
    ERROR_CHECK(TAG, readRegisters(REG_CDATAL, raw, sizeof(raw)));

    RGBC rgbc;
    rgbc.c = raw[1] << 8 | raw[0];
    rgbc.r = raw[3] << 8 | raw[2];
    rgbc.g = raw[5] << 8 | raw[4];
    rgbc.b = raw[7] << 8 | raw[6];

    rgb_color_t out;
    uint16_t c = rgbc.c ? rgbc.c : 1;
    out.r = static_cast<uint8_t>((rgbc.r * 255) / c);
    out.g = static_cast<uint8_t>((rgbc.g * 255) / c);
    out.b = static_cast<uint8_t>((rgbc.b * 255) / c);

    return out;
}

rgb_color_t Colorsensor::readRGBOnce() {
    enable();

    uint16_t delay_ms = 0;
    switch (it)
    {
        case IntegrationTime::MS_2_4: delay_ms = 3; break;
        case IntegrationTime::MS_24:  delay_ms = 24; break;
        case IntegrationTime::MS_50:  delay_ms = 50; break;
        case IntegrationTime::MS_101: delay_ms = 101; break;
        case IntegrationTime::MS_154: delay_ms = 154; break;
        case IntegrationTime::MS_700: delay_ms = 700; break;
    }
    vTaskDelay(pdMS_TO_TICKS(delay_ms));

    uint8_t raw[8];
    ERROR_CHECK(TAG, readRegisters(REG_CDATAL, raw, sizeof(raw)));

    RGBC rgbc;
    rgbc.c = raw[1] << 8 | raw[0];
    rgbc.r = raw[3] << 8 | raw[2];
    rgbc.g = raw[5] << 8 | raw[4];
    rgbc.b = raw[7] << 8 | raw[6];

    disable();

    rgb_color_t out;
    uint16_t c = rgbc.c ? rgbc.c : 1;
    out.r = static_cast<uint8_t>((rgbc.r * 255) / c);
    out.g = static_cast<uint8_t>((rgbc.g * 255) / c);
    out.b = static_cast<uint8_t>((rgbc.b * 255) / c);

    return out;
}
