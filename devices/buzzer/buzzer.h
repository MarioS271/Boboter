/**
 * @file buzzer.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <hal/ledc_types.h>
#include <soc/gpio_num.h>

class Robot;

namespace Device {
    class Buzzer {
    private:
        static constexpr const char* TAG = "Device::Buzzer";

        static constexpr gpio_num_t BUZZER_PIN = GPIO_NUM_19;
        static constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_1;
        static constexpr ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_2;

        static constexpr uint16_t INITIAL_FREQUENCY = 100;
        static constexpr uint16_t DUTY_CYCLE = 256;

        static constexpr uint16_t FREQUENCY_MIN = 20;
        static constexpr uint16_t FREQUENCY_MAX = 15'000;

        Robot& robot;
        uint16_t current_frequency;
        bool turned_off;

    public:
        explicit Buzzer(Robot& robot);
        ~Buzzer();

        /**
         * @brief Sets up the necessary GPIO pin and LEDC channel
         */
        void initialize();

        /**
         * @brief Set buzzer to a given frequency
         *
         * @param frequency The frequency to play
         */
        void set_frequency(uint16_t frequency);

        /**
         * @brief Stops buzzer output until it's set again
         */
        void turn_off();
    };
}