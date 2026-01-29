/**
 * @file buzzer.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "buzzer.h"

#include "include/robot.h"

namespace Device {
    Buzzer::Buzzer(Robot& robot) :
        robot(robot),
        current_frequency(INITIAL_FREQUENCY),
        turned_off(true)
    {
        LOGI("Constructor called");
    }

    Buzzer::~Buzzer() {
        LOGI("Destructor called");
        turn_off();
    }

    void Buzzer::initialize() {
        robot.ledc.add_timer(
            HAL::LEDC::timer_config_t{
                .timer = LEDC_TIMER,
                .frequency = INITIAL_FREQUENCY,
                .resolution = LEDC_TIMER_10_BIT
            }
        );

        robot.ledc.add_channel(
            HAL::LEDC::channel_config_t{
                .channel = LEDC_CHANNEL,
                .timer = LEDC_TIMER,
                .gpio_pin = BUZZER_PIN,
                .duty = DUTY_CYCLE
            }
        );

        turn_off();
    }

    void Buzzer::set_frequency(const uint16_t frequency) {
        if (frequency < FREQUENCY_MIN || frequency > FREQUENCY_MAX) {
            LOGW("Frequency is out of reasonable range, stopping buzzer output");
            turn_off();
            return;
        }

        if (current_frequency != frequency) {
            current_frequency = frequency;
            robot.ledc.set_frequency(LEDC_TIMER, frequency);
        }

        if (turned_off) {
            turned_off = false;
            robot.ledc.set_duty(LEDC_CHANNEL, DUTY_CYCLE);
        }
    }

    void Buzzer::turn_off() {
        turned_off = true;

        robot.ledc.set_duty(LEDC_CHANNEL, 0);
    }
}
