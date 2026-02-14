/**
 * @file motors.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "motors.h"

#include "include/robot/robot.h"

namespace Device {
    Motors::Motors(Robot& robot) :
        robot(robot),
        inverse_direction{ true, false }
    {
        LOGD("Constructor called");
    }

    Motors::~Motors() {
        LOGD("Destructor called");

        hard_stop(motor_id_t::LEFT);
        hard_stop(motor_id_t::RIGHT);
    }

    void Motors::initialize() {
        using enum motor_id_t;

        const motor_id_t motors_to_initialize[] = { LEFT, RIGHT };

        robot.ledc.add_timer(
            HAL::LEDC::timer_config_t{
                .timer = LEDC_TIMER,
                .frequency = 10'000,
                .resolution = LEDC_TIMER_10_BIT
            }
        );

        for (const auto motor : motors_to_initialize) {
            robot.gpio.add(
                HAL::GPIO::pin_config_t{
                    .gpio_pin = (motor == LEFT ? LEFT_DIRECTION_PIN : RIGHT_DIRECTION_PIN),
                    .mode = GPIO_MODE_OUTPUT,
                    .pull_mode = GPIO_FLOATING,
                    .intr_type = GPIO_INTR_DISABLE
                }
            );

            robot.ledc.add_channel(
                HAL::LEDC::channel_config_t{
                    .channel = (motor == LEFT ? LEFT_LEDC_CHANNEL : RIGHT_LEDC_CHANNEL),
                    .timer = LEDC_TIMER,
                    .gpio_pin = (motor == LEFT ? LEFT_SPEED_PIN : RIGHT_SPEED_PIN),
                    .duty = 0
                }
            );
        }

        LOGI("Initialized Device::Motors");
    }

    void Motors::stop(const motor_id_t motor_id, const uint16_t ramp_time) const {
        using enum motor_id_t;
        using enum motor_direction_t;

        auto locked_data = robot.data.lock();

        if (motor_id == LEFT) {
            locked_data->motors.target_speed_left = 0;
            locked_data->motors.ramp_time_left = ramp_time;
        } else {
            locked_data->motors.target_speed_right = 0;
            locked_data->motors.ramp_time_right = ramp_time;
        }
    }

    void Motors::hard_stop(const motor_id_t motor_id) const {
        using enum motor_id_t;

        auto locked_data = robot.data.lock();

        if (motor_id == LEFT) {
            locked_data->motors.target_speed_left = 0;
            locked_data->motors.ramp_time_left = 0;
        } else {
            locked_data->motors.target_speed_right = 0;
            locked_data->motors.ramp_time_right = 0;
        }
    }

    void Motors::set_speed(const motor_id_t motor_id, const uint16_t speed, const uint16_t ramp_time) const {
        using enum motor_id_t;

        auto locked_data = robot.data.lock();

        if (motor_id == LEFT) {
            locked_data->motors.target_speed_left = speed;
            locked_data->motors.ramp_time_left = ramp_time;
        } else {
            locked_data->motors.target_speed_right = speed;
            locked_data->motors.ramp_time_right = ramp_time;
        }
    }

    void Motors::set_direction(const motor_id_t motor_id, const motor_direction_t direction, const uint16_t ramp_time) const {
        using enum motor_id_t;

        auto locked_data = robot.data.lock();

        if (motor_id == LEFT) {
            locked_data->motors.direction_left = direction;
        } else {
            locked_data->motors.direction_right = direction;
        }
    }
}
