/**
 * @file battery.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <esp_adc/adc_oneshot.h>
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Battery {
    private:
        static constexpr const char* TAG = "Device::Battery";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_BATTERY;

        static constexpr adc_channel_t ADC_CHANNEL = ADC_CHANNEL_3;  // GPIO 39
        static constexpr uint8_t NUM_SAMPLES = 50;

        Robot& robot;

        int8_t percentage;
        uint16_t voltage;

    public:
        explicit Battery(Robot& robot);
        ~Battery();

        /**
         * @brief Configures the needed ADC channel
         */
        void initialize();

        /**
         * @brief Read and calculate current battery state
         */
        void update();

        /**
         * @brief Returns the current battery percentage in percent
         * @note The percentage can go negative if the measured voltage drops below 3300 mV
         *
         * @return The current percentage from ~0% and up to 100%
         */
        [[nodiscard]] int8_t get_percentage() const { return percentage; }

        /**
         * @brief Returns the current battery voltage
         *
         * @return The voltage in millivolts
         */
        [[nodiscard]] uint16_t get_millivolts() const { return voltage; }
    };
}
