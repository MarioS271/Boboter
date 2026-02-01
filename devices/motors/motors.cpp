/**
 * @file motors.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "motors.h"

#include "include/robot.h"

namespace Device {
    Motors::Motors(Robot& robot) :
        robot(robot),
        inverse_direction{}
    {
        LOGI("Constructor called");
    }

    Motors::~Motors() {
        LOGI("Destructor called");

        hard_stop(motor_id_t::LEFT);
        hard_stop(motor_id_t::RIGHT);
    }

    void Motors::initialize() {
        const motor_id_t motors_to_initialize = {motor_id_t::LEFT, motor_id_t::RIGHT};

        robot.ledc.add_timer(
            HAL::LEDC::timer_config_t{
                .timer = LEDC_TIMER_0,
                .frequency = 10'000,
                .resolution = LEDC_TIMER_12_BIT
            }
        );

        for (const auto motor : motors_to_initialize) {
            robot.gpio.add(
                HAL::GPIO::pin_config_t{
                    .gpio_pin = (motor == motor_id_t::LEFT ? LEFT_DIRECTION_PIN : RIGHT_DIRECTION_PIN),
                    .mode = GPIO_MODE_OUTPUT,
                    .pull_mode = GPIO_FLOATING,
                    .intr_type = GPIO_INTR_DISABLE
                }
            );

            robot.ledc.add_channel(
                HAL::LEDC::channel_config_t{
                    .channel = (motor == motor_id_t::LEFT ? LEDC_CHANNEL_0 : LEDC_CHANNEL_1),
                    .gpio_pin = (motor == motor_id_t::LEFT ? LEFT_SPEED_PIN : RIGHT_SPEED_PIN),
                    .timer = LEDC_TIMER_0,
                    .duty = 0
                }
            );
        }
    }

    // TODO: finish this crap cause its 22:01 and i wanna sleep :(
}
