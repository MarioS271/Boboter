/**
 * @file encoder.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include "encoder_types.hpp"

#include <atomic>
#include <driver/gpio.h>
#include <esp_attr.h>

namespace Boboter::Libs::Encoder {
    namespace Config {
        constexpr gpio_num_t ENCODER_LEFT_PIN = GPIO_NUM_27;
        constexpr gpio_num_t ENCODER_RIGHT_PIN = GPIO_NUM_14;
    }
    
    namespace Internal {
        void IRAM_ATTR encoder_isr_handler(void* arg);
    }

    class Encoder {
    private:
        static constexpr const char* TAG = "Libs::Encoder";
        
        Boboter::Types::Encoder::Id encoder_id;
        gpio_num_t encoder_pin;

        static bool isr_service_installed;
        
        friend void Internal::encoder_isr_handler(void* arg);
        std::atomic<int32_t> pulse_count;
    
    public:
        explicit Encoder(Boboter::Types::Encoder::Id encoder_id);
        ~Encoder() = default;
    
        /**
         * @brief Gets the current count of pulses since last reset
         *
         * @return (int32_t) The pulse count
        */
        int32_t get_pulse_count() const { return pulse_count; }

        /**
         * @brief Resets the pulse count back to zero
         *
         * @return (void)  
        */
        void reset_pulse_count() { pulse_count = 0; }
    };
}