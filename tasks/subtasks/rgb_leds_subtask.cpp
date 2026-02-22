/**
 * @file rgb_leds_subtask.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "subtasks.h"

#include <esp_random.h>
#include "helpers/predef_colors.h"
#include "include/robot.h"

constexpr uint8_t POLL_DELAY_MS = 10;
constexpr uint32_t RANDOM_COLORS_BLINK_INTERVAL_US = 1'000'000;

namespace Task::Subtask {
    void rgb_leds_subtask_init() {}

    uint32_t rgb_leds_subtask() {
        using enum Robot::rgb_leds_mode_t;
        using enum Device::Leds::led_id_t;

        static Robot& robot = Robot::get_instance();
        static Robot::rgb_leds_mode_t current_mode = robot.data->leds.rgb_leds_mode;
        static bool changed = true;
        static int64_t last_run_time = 0;

        if (const Robot::rgb_leds_mode_t new_mode = robot.data->leds.rgb_leds_mode;
            new_mode != current_mode)
        {
            changed = true;
            current_mode = new_mode;
        }

        switch (current_mode) {
            case OFF: {
                if (changed) {
                    changed = false;

                    robot.leds.turn_all_off();
                }
                return POLL_DELAY_MS;
            }

            case HEADLIGHTS: {
                if (changed) {
                    changed = false;

                    robot.leds.set_color(FRONT_LEFT, Colors::WHITE);
                    robot.leds.set_color(FRONT_RIGHT, Colors::WHITE);
                    robot.leds.set_color(BACK_LEFT, Colors::RED);
                    robot.leds.set_color(BACK_RIGHT, Colors::RED);
                    robot.leds.update();
                }
                return POLL_DELAY_MS;
            }

            case RANDOM_COLORS: {
                const int64_t current_time = esp_timer_get_time();
                if (current_time - last_run_time > RANDOM_COLORS_BLINK_INTERVAL_US) {
                    last_run_time = current_time;

                    robot.leds.set_color(FRONT_LEFT, Colors::LIST[esp_random() % Colors::NUM_COLORS]);
                    robot.leds.set_color(FRONT_RIGHT, Colors::LIST[esp_random() % Colors::NUM_COLORS]);
                    robot.leds.set_color(BACK_LEFT, Colors::LIST[esp_random() % Colors::NUM_COLORS]);
                    robot.leds.set_color(BACK_RIGHT, Colors::LIST[esp_random() % Colors::NUM_COLORS]);
                    robot.leds.update();
                }
                return POLL_DELAY_MS;
            }

            case POLICE: {

                return POLL_DELAY_MS;
            }
        }

        return POLL_DELAY_MS;
    }
}
