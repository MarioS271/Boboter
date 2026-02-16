/**
 * @file colorsensor.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "colorsensor.h"

#include "helpers/delay.h"
#include "helpers/predef_colors.h"
#include "lib/logger/logger.h"
#include "include/robot.h"

#include <cmath> // For sqrt or pow, if needed. For squared distance, not strictly necessary, but good practice.
#include <limits> // For std::numeric_limits

namespace Device {
    // Helper function to calculate squared Euclidean distance between two colors
    // We are comparing a predefined color (rgb_color_t) with measured (uint8_t r, g, b)
    static float calculate_squared_distance(const rgb_color_t& c1, uint8_t r2, uint8_t g2, uint8_t b2) {
        float dr = static_cast<float>(c1.r) - r2;
        float dg = static_cast<float>(c1.g) - g2;
        float db = static_cast<float>(c1.b) - b2;
        return dr * dr + dg * dg + db * db;
    }

    Colorsensor::Colorsensor(Robot& robot) :
        robot(robot),
        color(Colors::OFF),
        brightness(0)
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

        write_register(REG_ENABLE, CMD_ENABLE_PON);
        delay(5);
        write_register(REG_ENABLE, CMD_ENABLE_PON | CMD_ENABLE_AEN);

        LOGI("Initialized Device::Colorsensor");
    }

    void Colorsensor::measure() {
        uint8_t command = COMMAND_BIT | AUTO_INC | REG_CLEAR_LOW;
        uint8_t raw[8];

        robot.i2c.write_read(device_handle, &command, 1, raw, 8);

        uint16_t raw_clear = (raw[1] << 8) | raw[0];
        uint16_t raw_red = (raw[3] << 8) | raw[2];
        uint16_t raw_green = (raw[5] << 8) | raw[4];
        uint16_t raw_blue = (raw[7] << 8) | raw[6];

        if (raw_clear == 0) {
            LOGW("Received clear value of zero, cancelling measurement to avoid division by zero");
            return;
        }

        // Calculate normalized color components (0.0 to 1.0 floats)
        // These are true ratios, needed for accurate color representation.
        float red_ratio_f = static_cast<float>(raw_red) / raw_clear;
        float green_ratio_f = static_cast<float>(raw_green) / raw_clear;
        float blue_ratio_f = static_cast<float>(raw_blue) / raw_clear;

        // Scale ratios to 0-255 range for comparison with predefined colors
        uint8_t current_r_255 = static_cast<uint8_t>(red_ratio_f * 255.0f);
        uint8_t current_g_255 = static_cast<uint8_t>(green_ratio_f * 255.0f);
        uint8_t current_b_255 = static_cast<uint8_t>(blue_ratio_f * 255.0f);

        // Find the closest predefined color using squared Euclidean distance
        float min_distance_sq = std::numeric_limits<float>::max();
        rgb_color_t detected_color = Colors::OFF; // Default to OFF if no match or error

        for (uint8_t i = 0; i < Colors::NUM_COLORS; ++i) {
            const rgb_color_t& predef_color = Colors::LIST[i];
            float distance_sq = calculate_squared_distance(predef_color, current_r_255, current_g_255, current_b_255);

            if (distance_sq < min_distance_sq) {
                min_distance_sq = distance_sq;
                detected_color = predef_color;
            }
        }

        this->color = detected_color; // Update the color member variable

        // --- Brightness Calculation Logic (Original Approach) ---
        // The original brightness calculation used integer division results for red, green, blue.
        // To maintain the original behavior for brightness, we use these integer ratios.
        // If the brightness formula should use the float ratios scaled to 0-255,
        // `current_r_255`, `current_g_255`, `current_b_255` should be used instead.
        // For now, preserving the closest interpretation of the original brightness calculation.
        uint16_t red_int_ratio = raw_red / raw_clear;
        uint16_t green_int_ratio = raw_green / raw_clear;
        uint16_t blue_int_ratio = raw_blue / raw_clear;

        brightness = static_cast<uint8_t>((-0.32466f * red_int_ratio) + (1.57833f * green_int_ratio) + (-0.73191f * blue_int_ratio));

        LOGV("Measured R:%u G:%u B:%u (scaled to 0-255), Brightness: %u, Detected Color: R:%u G:%u B:%u",
             current_r_255, current_g_255, current_b_255, brightness, this->color.r, this->color.g, this->color.b);
    }
}