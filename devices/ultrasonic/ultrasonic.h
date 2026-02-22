/**
 * @file ultrasonic.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <cstdint>
#include <soc/gpio_num.h>
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Ultrasonic {
    private:
        static constexpr const char* TAG = "Device::Ultrasonic";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_ULTRASONIC;

        static constexpr gpio_num_t TRIGGER_PIN = GPIO_NUM_25;
        static constexpr gpio_num_t ECHO_PIN = GPIO_NUM_26;

        static constexpr uint16_t ECHO_TIMEOUT_MS = 500;
        static constexpr uint8_t ECHO_POLL_DELAY_MS = 10;

        Robot& robot;
        float distance;

    public:
        explicit Ultrasonic(Robot& robot);
        ~Ultrasonic();

        /**
         * @brief Sets up the necessary GPIO pins
         */
        void initialize();

        /**
         * @brief Measures the distance of any object to the sensor
         * @note If the read times out, the internal distance variable is set to @c -1
         */
        void measure();

        /**
         * @brief Returns the distance in centimeters
         * @note If the previous read timed out or distance was never read, this will return @c -1
         *
         * @return The distance in cm
         */
        [[nodiscard]] float get_distance() const { return distance; }
    };
}
