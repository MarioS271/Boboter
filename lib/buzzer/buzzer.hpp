/**
 * @file buzzer.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <cstdint>
#include <driver/gpio.h>
#include <driver/ledc.h>

namespace Boboter::Libs::Buzzer {
    namespace Config {
        constexpr gpio_num_t BUZZER_PIN = GPIO_NUM_19;
        constexpr uint8_t DUTY_CYCLE = 512;

        constexpr ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;
        constexpr ledc_timer_bit_t LEDC_TIMER_BITS = LEDC_TIMER_10_BIT;
        constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_1;
        constexpr ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_2;
        constexpr soc_periph_ledc_clk_src_legacy_t LEDC_CLOCK = LEDC_AUTO_CLK;
    }

    class Buzzer {
    private:
        static constexpr const char* TAG = "Libs::Buzzer";
    
    public:
        explicit Buzzer();
        ~Buzzer() = default;

        /**
         * @brief Plays a specific frequency on the buzzer
         *
         * @return (void)  
         *
         * @param frequency The frequency to play
        */
        void play_frequency(uint16_t frequency);

        /**
         * @brief Stops buzzer output
         *
         * @return (void)  
        */
        void stop();
    };
}