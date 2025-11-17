// MOTOR_RAMP_CPP
// Boboter
// (C) MarioS271 2025

#include "motors.h"

#include "delay.h"

void Motor::rampTask(void* params) {
    Motor* motor = static_cast<Motor*>(params);

    while (true) {
        if (motor->current_speed != motor->target_speed) {
            int16_t current = static_cast<int16_t>(motor->current_speed);
            int16_t target = static_cast<int16_t>(motor->target_speed);

            if (current < target) {
                current += RAMP_STEP;
                if (current > target) current = target;
            } else if (current > target) {
                current -= RAMP_STEP;
                if (current < target) current = target;
            }

            current = current > 1023 ? 1023 : current;
            motor->current_speed = current;
            ledc_set_duty(LEDC_SPEED_MODE, motor->ledc_channel, static_cast<uint16_t>(current));
            ledc_update_duty(LEDC_SPEED_MODE, motor->ledc_channel);
        }

        delay(RAMP_INTERVAL_MS);
    }
}