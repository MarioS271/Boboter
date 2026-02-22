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
    [[noreturn]] void drive_task(void* params);
    [[noreturn]] void ramp_task(void* params);
    [[noreturn]] void sensor_fusion_task(void* params);
    [[noreturn]] void sensor_reader_task(void* params);
    [[noreturn]] void action_task(void* params);
    [[noreturn]] void app_backend_task(void* params);
    [[noreturn]] void gamepad_task(void* params);
    [[noreturn]] void log_task(void* params);
    [[noreturn]] void subtask_scheduler_task(void* params);

    [[noreturn]] void test_task(void* params);
}