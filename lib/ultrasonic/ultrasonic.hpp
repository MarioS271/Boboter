/**
 * @file ultrasonic.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <driver/gpio.h>

namespace Boboter::Libs::Ultrasonic {
    namespace Config {
        constexpr gpio_num_t TRIGGER_PIN = GPIO_NUM_25;
        constexpr gpio_num_t ECHO_PIN = GPIO_NUM_26;
    }

    class Ultrasonic {
    private:
        static constexpr const char* TAG = "Libs::Ultrasonic";
    
    public:
        explicit Ultrasonic();
        ~Ultrasonic() = default;
        
        /**
         * @brief Measures and returns the distance from the sensor
         *
         * @return (float) Distance in cm
        */
        float measureCm();
    };
}
