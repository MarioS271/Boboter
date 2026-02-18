/**
 * @file linefollower.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <soc/gpio_num.h>
#include <esp_adc/adc_oneshot.h>
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Linefollower {
    public:
        enum class linefollower_result_t : uint8_t;

    private:
        static constexpr const char* TAG = "Device::Linefollower";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_LINEFOLLOWER;

        static constexpr gpio_num_t LF_RIGHT_LEFT_PIN = GPIO_NUM_12;
        static constexpr adc_channel_t ADC_CHANNEL = ADC_CHANNEL_0;

        Robot& robot;
        linefollower_result_t left_sensor_color;
        linefollower_result_t right_sensor_color;

    public:
        static constexpr uint16_t REFLECTIVITY_THRESHOLD = 2300;

        enum class linefollower_id_t : uint8_t {
            LEFT = 1,
            RIGHT = 0
        };

        enum class linefollower_result_t : uint8_t {
            BLACK = 0,
            WHITE = 1
        };

    public:
        explicit Linefollower(Robot& robot);
        ~Linefollower();

        /**
         * @brief Sets up the necessary GPIO pin and ADC channel
         */
        void initialize();

        /**
         * @brief Measures the currently seen color from both sensors
         */
        void measure();

        /**
         * @brief Get the last measured color from the left sensor
         *
         * @return The color last measured by the right sensor
         */
         [[nodiscard]] linefollower_result_t get_left_sensor_color() const { return left_sensor_color; }

        /**
         * @brief Get the last measured color from the right sensor
         *
         * @return The color last measured by the right sensor
         */
        [[nodiscard]] linefollower_result_t get_right_sensor_color() const { return left_sensor_color; }
    };
}