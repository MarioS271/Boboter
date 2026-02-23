/**
 * @file colorsensor.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "colorsensor.h"

#include "helpers/delay.h"
#include "helpers/square_value.h"
#include "helpers/map_value.h"
#include "helpers/predef_colors.h"
#include "lib/logger/logger.h"
#include "include/robot.h"

namespace Device {
    Colorsensor::Colorsensor(Robot& robot) :
        robot(robot),
        device_handle(nullptr),
        color(Colors::BLACK)
    {
        LOGD("Constructor called");
    }

    Colorsensor::~Colorsensor() {
        LOGD("Destructor called");
        write_register(REG_ENABLE, 0x00);
    }

    void Colorsensor::initialize() {
        device_handle = robot.i2c.add_device(
            HAL::I2C::device_config_t{
                .address = I2C_ADDRESS,
                .clock_speed = 400'000,
                .address_bit_length = I2C_ADDR_BIT_LEN_7
            }
        );

        if (device_handle == nullptr) {
            LOGE("Failed to add colorsensor I2C device");
            halt_execution();
        }

        const uint8_t device_id = read_register(REG_ID);
        if (device_id != TCS34725_DEVICE_ID) {
            LOGE("TCS34725 not found! Expected id 0x44, got 0x%02x", device_id);
            halt_execution();
        }
        LOGI("TCS34725 with id 0x%02x detected", device_id);

        write_register(REG_ATIME, 0xEB);        // 100ms integration time
        write_register(REG_CONTROL, GAIN_16X);  // 16x gain

        write_register(REG_ENABLE, CMD_ENABLE_PON);
        delay(5);
        write_register(REG_ENABLE, CMD_ENABLE_PON | CMD_ENABLE_AEN);

        LOGI("Initialized Device::Colorsensor");
    }

    void Colorsensor::measure() {
        constexpr uint8_t command = COMMAND_BIT | AUTO_INC | REG_CLEAR_LOW;
        uint8_t raw[8];

        robot.i2c.write_read(device_handle, &command, 1, raw, 8);

        const uint16_t raw_clear = (raw[1] << 8) | raw[0];
        const uint16_t raw_red = (raw[3] << 8) | raw[2];
        const uint16_t raw_green = (raw[5] << 8) | raw[4];
        const uint16_t raw_blue = (raw[7] << 8) | raw[6];

        if (raw_clear == 0) {
            LOGW("Received clear value of zero, cancelling measurement to avoid division by zero");
            return;
        }

        const float red_ratio = (static_cast<float>(raw_red) / raw_clear);
        const float green_ratio = (static_cast<float>(raw_green) / raw_clear);
        const float blue_ratio = (static_cast<float>(raw_blue) / raw_clear);

        const rgb_color_t mapped_color_reading = {
            map_value(red_ratio, 0.0f, 1.0f, 0, 255),
            map_value(green_ratio, 0.0f, 1.0f, 0, 255),
            map_value(blue_ratio, 0.0f, 1.0f, 0, 255)
        };

        uint32_t lowest_distance_score{UINT32_MAX};
        uint8_t index_of_nearest_color{};

        for (uint8_t i = 0; i < Colors::NUM_COLORS_WITH_BLACK; ++i) {
            const int32_t difference_red = mapped_color_reading.r - Colors::LIST_WITH_BLACK[i].r;
            const int32_t difference_green = mapped_color_reading.g - Colors::LIST_WITH_BLACK[i].g;
            const int32_t difference_blue = mapped_color_reading.b - Colors::LIST_WITH_BLACK[i].b;

            const uint32_t distance_score = square(difference_red) + square(difference_green) + square(difference_blue);
            if (distance_score < lowest_distance_score) {
                lowest_distance_score = distance_score;
                index_of_nearest_color = i;
            }
        }

        this->color = Colors::LIST_WITH_BLACK[index_of_nearest_color];

        LOGV("Measured mapped ratios of red=%u green=%u blue=%u",
            mapped_color_reading.r, mapped_color_reading.g, mapped_color_reading.b);
    }
}