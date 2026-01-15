/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/flags.h"
#include "include/constants.h"
#include "include/robot.h"
#include "lib/logger/logger.h"
#include "tasks/tasks.h"

#include "helpers/delay.h"

extern "C" void app_main() {
    constexpr const char* TAG = "Main";

    esp_log_level_set("gpio", ESP_LOG_NONE);

    LOGI("=== Boboter is starting ===");
    LOGI("Firmware v%s by %s", Constants::VERSION, Constants::AUTHORS);
    LOGI("Hello, World!");
    LOGI("===========================");

    Robot& robot = Robot::get_instance();

    robot.adc.configure(
        HAL::ADC::controller_config_t{
            .attenuation = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_12,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
            .clock_source = ADC_RTC_CLK_SRC_DEFAULT
        }
    );
    robot.i2c.configure(
        HAL::I2C::bus_config_t{
            .port = I2C_NUM_0,
            .sda_pin = GPIO_NUM_21,
            .scl_pin = GPIO_NUM_22
        }
    );
    robot.i2c.scan_for_devices();

    robot.gpio.add(
        HAL::GPIO::pin_config_t{
            .gpio_pin = Robot::STATUS_LED_PIN,
            .mode = GPIO_MODE_OUTPUT,
            .pull_mode = GPIO_FLOATING,
            .intr_type = GPIO_INTR_DISABLE
        }
    );
    robot.gpio.add(
        HAL::GPIO::pin_config_t{
            .gpio_pin = Robot::BOTTOM_LED_PIN,
            .mode = GPIO_MODE_OUTPUT,
            .pull_mode = GPIO_FLOATING,
            .intr_type = GPIO_INTR_DISABLE
        }
    );

    robot.set_status_led(true);
    robot.set_bottom_led(false);

    robot.battery.initialize();
    robot.leds.initialize();
    robot.display.initialize();
    robot.buttons.initialize();

    LOGI("Switching logging mode from real mode to queue mode");
    Logger::get_instance().switch_to_queue_logging(
        xQueueCreate(64, sizeof(Logger::log_item))
    );

    // Secure Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::secure_task,
            .task_name = "SecureTask",
            .stack_depth = 2048,
            .params_for_task = nullptr,
            .priority = 24,
            .created_task_handle = nullptr,
            .core_id = 0
        }
    );

    // System Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::system_task,
            .task_name = "SystemTask",
            .stack_depth = 2048,
            .params_for_task = nullptr,
            .priority = 20,
            .created_task_handle = nullptr,
            .core_id = 0
        }
    );

    // PID Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::pid_task,
            .task_name = "PIDTask",
            .stack_depth = 4096,
            .params_for_task = nullptr,
            .priority = 15,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    // Ramp Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::ramp_task,
            .task_name = "RampTask",
            .stack_depth = 4096,
            .params_for_task = nullptr,
            .priority = 14,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    // IO Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::io_task,
            .task_name = "IOTask",
            .stack_depth = 4096,
            .params_for_task = nullptr,
            .priority = 10,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    // Log Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::log_task,
            .task_name = "LogTask",
            .stack_depth = 4096,
            .params_for_task = nullptr,
            .priority = 6,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    // LEDs Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::leds_task,
            .task_name = "LedsTask",
            .stack_depth = 2048,
            .params_for_task = nullptr,
            .priority = 3,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    // Buzzer Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::buzzer_task,
            .task_name = "BuzzerTask",
            .stack_depth = 2048,
            .params_for_task = nullptr,
            .priority = 1,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    LOGI("Created all tasks successfully");
    vTaskDelete(nullptr);
}