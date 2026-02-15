/**
 * @file encoders.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <atomic>
#include <esp_attr.h>
#include <soc/gpio_num.h>
#include "include/log_sources.h"

class Robot;

namespace Device {
    class Encoders {
    private:
        static constexpr const char* TAG = "Device::Encoders";
        static constexpr log_source LOG_SOURCE = LOG_SOURCE_DEVICE_ENCODERS;

        static constexpr gpio_num_t LEFT_ENCODER_PIN = GPIO_NUM_27;
        static constexpr gpio_num_t RIGHT_ENCODER_PIN = GPIO_NUM_14;

        struct isr_context_t {
            Encoders* instance;
            uint8_t encoder_index;
        };

        Robot& robot;
        std::atomic<int32_t> pulse_count[2];
        isr_context_t isr_contexts[2];

    public:
        static void IRAM_ATTR encoder_isr_handler(void* arg);

        enum class encoder_id_t : uint8_t {
            LEFT = 0,
            RIGHT = 1,
        };

    public:
        explicit Encoders(Robot& robot);
        ~Encoders();

        /**
         * @brief Sets up the necessary GPIO pins
         */
        void initialize();

        /**
         * @brief Gets the pulse count of the given encoder
         *
         * @return The pulse count
         *
         * @param encoder_id The id of the encoder of which to get the pulse count
         */
        [[nodiscard]] int32_t get_pulse_count(encoder_id_t encoder_id) const {
            return pulse_count[static_cast<uint8_t>(encoder_id)];
        }

        /**
         * @brief Resets the pulse count of the given encoder back to zero
         *
         * @param encoder_id The id of the encoder of which to reset the pulse count
         */
        void reset_pulse_count(encoder_id_t encoder_id) {
            pulse_count[static_cast<uint8_t>(encoder_id)] = 0;
        }
    };
}
