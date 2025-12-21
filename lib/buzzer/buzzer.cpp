/**
 * @file buzzer.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "buzzer.hpp"

#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"

namespace Boboter::Libs::Buzzer {
    Buzzer::Buzzer() {
        using namespace Config;
        using namespace Boboter::Libs::Logger;
        using namespace Boboter::Libs::Error;

        ledc_timer_config_t timer_config = {
            .speed_mode = LEDC_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_BITS,
            .timer_num = LEDC_TIMER,
            .freq_hz = 1000,
            .clk_cfg = LEDC_CLOCK
        };
        ERROR_CHECK(TAG, ledc_timer_config(&timer_config));

        ledc_channel_config_t channel_config = {
            .gpio_num = BUZZER_PIN,
            .speed_mode = LEDC_SPEED_MODE,
            .channel = LEDC_CHANNEL,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER,
            .duty = 0,
            .hpoint = 0
        };
        ERROR_CHECK(TAG, ledc_channel_config(&channel_config));

        LOGI(TAG, "Initialized Buzzer");
    }

    void Buzzer::play_frequency(uint16_t frequency) {
        using namespace Config;
        using namespace Boboter::Libs::Error;

        if (frequency < 20 || frequency > 15'000) {
            stop();
            return;
        }

        WARN_CHECK(TAG, ledc_set_freq(LEDC_SPEED_MODE, LEDC_TIMER, frequency));

        WARN_CHECK(TAG, ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL, DUTY_CYCLE));
        WARN_CHECK(TAG, ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL));
    }

    void Buzzer::stop() {
        using namespace Config;
        using namespace Boboter::Libs::Error;

        WARN_CHECK(TAG, ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL, 0));
        WARN_CHECK(TAG, ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL));
    }
}