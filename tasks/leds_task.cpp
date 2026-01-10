/**
 * @file leds_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include <esp_random.h>
#include "include/robot.h"
#include "helpers/delay.h"
#include "helpers/predef_colors.h"
#include "lib/logger/logger.h"

static constexpr const char* TAG = "Task::leds_task";
static constexpr uint8_t POLLING_DELAY_WAIT_MS = 10;

/**
 * @brief A function to wait while also periodically polling if the LED mode changed
 *
 * @return Whether it detected a mode change (true) or not (false)
 */
static bool polling_delay_callback(const uint32_t ms, const bool& changed) {
    const Robot& robot = Robot::get_instance();

    const Robot::rgb_leds_mode_t initial_mode = robot.data.leds.rgb_leds_mode;
    uint32_t elapsed = 0;

    while (elapsed < ms && robot.data.leds.rgb_leds_mode == initial_mode) {
        delay(POLLING_DELAY_WAIT_MS);
        elapsed += POLLING_DELAY_WAIT_MS;
    }

    if (elapsed < ms) [[unlikely]] {
        return true;
    }
    return false;
}

/**
 * @brief Macro that wraps @c polling_delay_callback(ms) and checks if it needs to
 *        skip to next iteration of the loop
 */
#define polling_delay(ms) if (polling_delay_callback(ms, changed)) continue;


namespace Task {
    /**
     * @brief Manages the NeoPixel RGB LEDs and executes lighting routines.
     */
    [[noreturn]] void leds_task(void* params) {
        Robot& robot = Robot::get_instance();

        Robot::rgb_leds_mode_t current_mode = robot.data.leds.rgb_leds_mode;
        Robot::rgb_leds_mode_t previous_mode = current_mode;
        bool changed = false;

        while (true) {
            using enum Robot::rgb_leds_mode_t;

            current_mode = robot.data.leds.rgb_leds_mode;

            switch (current_mode) {
                case OFF:
                    robot.leds.turn_all_off();
            }

            for (int i = 0; i < 4; ++i) {
                const uint32_t color_index = esp_random() % Colors::NUM_COLORS;

                robot.leds.set_color(
                    static_cast<Device::Leds::led_id_t>(i),
                    Colors::LIST[color_index]
                );
            }

            polling_delay(1000);
        }
    }
}