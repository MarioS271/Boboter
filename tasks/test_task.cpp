/**
 * @file test_task.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
*/

#include "tasks.h"

#include "helpers/delay.h"
#include "helpers/predef_colors.h"
#include "lib/logger/logger.h"
#include "include/robot.h"

constexpr const char* TAG = "Task::test_task";
constexpr float MAX_DISPLAY_DISTANCE = 60.0;
constexpr float MIN_DISPLAY_DISTANCE = 5.0;

namespace Task {
    [[noreturn]] void test_task(void* params) {
        using namespace Device;

        Robot& robot = Robot::get_instance();

        // Leds Test
        for (const auto color : Colors::LIST) {
            robot.leds.set_color_all(color);
            delay(250);
        }
        delay(250);

        // Motor Test
        {
            using enum Motors::motor_id_t;
            using enum Motors::motor_direction_t;

            constexpr Motors::motor_direction_t directions[] = { FORWARD, BACKWARD };

            for (const auto direction : directions) {
                robot.motors.set_direction(LEFT, direction);
                robot.motors.set_direction(RIGHT, direction);
                robot.motors.set_speed(LEFT, Motors::MAX_MOTOR_SPEED);
                robot.motors.set_speed(RIGHT, Motors::MAX_MOTOR_SPEED);
                delay(3000);
                robot.motors.stop(LEFT);
                robot.motors.stop(RIGHT);
                delay(1500);
            }
        }

        // Sensors Test
        while (true) {
            using enum Leds::led_id_t;
            using enum Linefollower::linefollower_result_t;

            robot.ultrasonic.measure();
            robot.colorsensor.measure();
            robot.linefollower.measure();

            float t = (robot.ultrasonic.get_distance() - MIN_DISPLAY_DISTANCE) / (MAX_DISPLAY_DISTANCE - MIN_DISPLAY_DISTANCE);
            t = t > 1.0 ? 1.0 : (t < 0 ? 0 : t);

            rgb_color_t ultrasonic_color{};
            if (t < 0.33) {
                const float subT = t / 0.33f;
                ultrasonic_color.r = 255;
                ultrasonic_color.g = static_cast<uint8_t>(255 * subT);
                ultrasonic_color.b = 0;
            } else if (t < 0.66) {
                const float subT = (t - 0.33f) / 0.33f;
                ultrasonic_color.r = static_cast<uint8_t>(255 * (1.0f - subT));
                ultrasonic_color.g = 255;
                ultrasonic_color.b = 0;
            } else {
                const float subT = (t - 0.66f) / 0.34f;
                ultrasonic_color.r = 0;
                ultrasonic_color.g = static_cast<uint8_t>(255 * (1.0f - subT));
                ultrasonic_color.b = static_cast<uint8_t>(255 * subT);
            }

            robot.leds.set_color(BACK_LEFT, robot.colorsensor.get_color());
            robot.leds.set_color(BACK_RIGHT, ultrasonic_color);

            robot.leds.set_color(FRONT_LEFT, robot.linefollower.get_left_sensor_color() == WHITE ? Colors::WHITE : Colors::BLACK);
            robot.leds.set_color(FRONT_RIGHT, robot.linefollower.get_right_sensor_color() == WHITE ? Colors::WHITE : Colors::BLACK);

            robot.leds.update();

            delay(250);
        }
    }
}