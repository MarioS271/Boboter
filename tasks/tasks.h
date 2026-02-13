/**
 * @file tasks.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

/**
 * @brief A namespace containing all the custom FreeRTOS tasks the robot uses
 */
namespace Task {
    [[noreturn]] void secure_task(void* params);
    [[noreturn]] void pid_task(void* params);
    [[noreturn]] void ramp_task(void* params);
    [[noreturn]] void io_task(void* params);
    [[noreturn]] void app_backend_task(void* params);
    [[noreturn]] void gamepad_task(void* params);
    [[noreturn]] void log_task(void* params);
    [[noreturn]] void rgb_leds_task(void* params);
    [[noreturn]] void status_led_task(void* params);
    [[noreturn]] void buzzer_task(void* params);

    [[noreturn]] void test_task(void* params);
}