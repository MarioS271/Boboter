/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "helpers/delay.h"
#include "lib/logger/logger.h"
#include "include/flags.h"
#include "include/constants.h"
#include "include/robot.h"
#include "tasks/tasks.h"

extern "C" void app_main() {
    constexpr const char* TAG = "Main";

    esp_log_level_set("gpio", ESP_LOG_NONE);

    LOGI("=== Boboter is starting ===");
    LOGI("Firmware v%s by %s", Constants::VERSION, Constants::AUTHORS);
    LOGI("Hello, World!");
    LOGI("===========================");

    Robot& robot = Robot::get_instance();

    robot.gpio.enable_interrupts();
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
    LOGI("Initialized GPIO pins for status LED and bottom LED");

    robot.set_bottom_led(true);

    robot.battery.initialize();
    robot.leds.initialize();
    robot.display.initialize();
    robot.buttons.initialize();
    robot.buzzer.initialize();
    robot.motors.initialize();
    robot.bumpers.initialize();
    robot.encoders.initialize();
    robot.ultrasonic.initialize();
    robot.colorsensor.initialize();
    robot.linefollower.initialize();

    LOGI("Switching logging mode from real mode to queue mode");
    Logger::get_instance().switch_to_queue_logging(
        xQueueCreate(64, sizeof(Logger::log_item))
    );

    auto locked_data = robot.data.lock();
    locked_data->leds.status_led_mode = Robot::status_led_mode_t::ON;
    locked_data->leds.rgb_leds_mode = Robot::rgb_leds_mode_t::RANDOM_COLORS;
    locked_data.unlock();

    // Secure Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::secure_task,
            .task_name = "SecureTask",
            .stack_size = 2048,
            .params_for_task = nullptr,
            .priority = 24,
            .created_task_handle = nullptr,
            .core_id = 0
        }
    );

    // Ramp Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::ramp_task,
            .task_name = "RampTask",
            .stack_size = 2048,
            .params_for_task = nullptr,
            .priority = 19,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    // Log Task
    robot.create_task(
        Robot::task_config_t{
            .task_function = Task::log_task,
            .task_name = "LogTask",
            .stack_size = 4096,
            .params_for_task = nullptr,
            .priority = 9,
            .created_task_handle = nullptr,
            .core_id = 1
        }
    );

    if constexpr (!Flags::ENABLE_TEST_MODE) {
        // Drive Task
        // robot.create_task(
        //     Robot::task_config_t{
        //         .task_function = Task::drive_task,
        //         .task_name = "PIDTask",
        //         .stack_size = 4096,
        //         .params_for_task = nullptr,
        //         .priority = 20,
        //         .created_task_handle = nullptr,
        //         .core_id = 1
        //     }
        // );

        // Sensor Fusion Task
        // robot.create_task(
        //     Robot::task_config_t{
        //         .task_function = Task::sensor_fusion_task,
        //         .task_name = "SensorFusionTask",
        //         .stack_size = 4096,
        //         .params_for_task = nullptr,
        //         .priority = 17,
        //         .created_task_handle = nullptr,
        //         .core_id = 1
        //     }
        // );

        // Sensor Reader Task
        robot.create_task(
            Robot::task_config_t{
                .task_function = Task::sensor_reader_task,
                .task_name = "SensorReaderTask",
                .stack_size = 4096,
                .params_for_task = nullptr,
                .priority = 16,
                .created_task_handle = nullptr,
                .core_id = 1
            }
        );

        // Action Task
        // robot.create_task(
        //     Robot::task_config_t{
        //         .task_function = Task::action_task,
        //         .task_name = "ActionTask",
        //         .stack_size = 4096,
        //         .params_for_task = nullptr,
        //         .priority = 14,
        //         .created_task_handle = nullptr,
        //         .core_id = 1
        //     }
        // );

        // Subtask Scheduler Task
        robot.create_task(
            Robot::task_config_t{
                .task_function = Task::subtask_scheduler_task,
                .task_name = "SubtaskSchedulerTask",
                .stack_size = 4096,
                .params_for_task = nullptr,
                .priority = 7,
                .created_task_handle = nullptr,
                .core_id = 1
            }
        );
    } else {
        // Test task
        robot.create_task(
            Robot::task_config_t{
                .task_function = Task::test_task,
                .task_name = "TestTask",
                .stack_size = 2048,
                .params_for_task = nullptr,
                .priority = 21,
                .created_task_handle = nullptr,
                .core_id = 1
            }
        );
    }

    LOGI("Created all tasks successfully");
    vTaskDelete(nullptr);
}