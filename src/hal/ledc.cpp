/**
 * @file ledc.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "include/hal/ledc.h"

#include <driver/gpio.h>
#include "types/smart_mutex.h"
#include "helpers/halt_execution.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace HAL::LEDC {
    Controller::Controller() :
        mutex(xSemaphoreCreateMutex())
    {
        if (mutex == nullptr) {
            LOGE("Failed to create mutex");
            halt_execution();
        }

        LOGD("Constructor called");
    }

    Controller::~Controller() {
        LOGD("Destructor called");

        shutdown();

        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
        }
    }

    void Controller::shutdown() {
        smart_mutex lock(mutex);

        for (const auto& channel : registered_channels) {
            ledc_stop(SPEED_MODE, channel.channel, 0);
            gpio_reset_pin(channel.gpio_pin);
        }

        for (const auto& timer : registered_timers) {
            ledc_timer_pause(SPEED_MODE, timer);

            ledc_timer_config_t deconfig = {
                .speed_mode = SPEED_MODE,
                .timer_num = timer,
                .deconfigure = true
            };
            ledc_timer_config(&deconfig);
        }

        registered_timers.clear();
        registered_channels.clear();

        LOGI("Shut down HAL::LEDC::Controller");
    }

    void Controller::add_timer(const timer_config_t& config) {
        smart_mutex lock(mutex);

        const ledc_timer_config_t timer_config = {
            .speed_mode = SPEED_MODE,
            .duty_resolution = config.resolution,
            .timer_num = config.timer,
            .freq_hz = config.frequency,
            .clk_cfg = LEDC_AUTO_CLK,
            .deconfigure = false
        };

        ERROR_CHECK(ledc_timer_config(&timer_config));
        registered_timers.push_back(config.timer);

        LOGD("Registered timer %hhu", config.timer);
    }

    void Controller::add_channel(const channel_config_t& config) {
        smart_mutex lock(mutex);

        if (GPIO_IS_VALID_OUTPUT_GPIO(config.gpio_pin)) {
            ERROR_CHECK(gpio_hold_dis(config.gpio_pin));
        }

        const ledc_channel_config_t channel_config = {
            .gpio_num = config.gpio_pin,
            .speed_mode = SPEED_MODE,
            .channel = config.channel,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = config.timer,
            .duty = config.duty,
            .hpoint = 0,
            .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
            .flags = { .output_invert = 0 }
        };

        ERROR_CHECK(ledc_channel_config(&channel_config));
        registered_channels.push_back(
            saved_channel_config_t{
                config.channel,
                config.gpio_pin
            }
        );

        LOGD("Registered channel %hhu on GPIO %hhu with timer %hhu", config.channel, config.gpio_pin, config.timer);
    }

    void Controller::set_frequency(const ledc_timer_t ledc_timer, const uint32_t frequency) const {
        smart_mutex lock(mutex);
        ERROR_CHECK(ledc_set_freq(SPEED_MODE, ledc_timer, frequency));

        LOGV("Set timer %hhu to frequency %lu", ledc_timer, frequency);
    }

    void Controller::set_duty(const ledc_channel_t ledc_channel, const uint32_t duty) const {
        smart_mutex lock(mutex);

        ERROR_CHECK(ledc_set_duty(SPEED_MODE, ledc_channel, duty));
        ERROR_CHECK(ledc_update_duty(SPEED_MODE, ledc_channel));

        LOGV("Set channel %hhu to duty %lu", ledc_channel, duty);
    }
}