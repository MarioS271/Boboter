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

#include <esp_random.h>
#include "helpers/delay.h"
#include "helpers/predef_colors.h"

void led_task(void* params) {
    Robot& robot = Robot::get_instance();

    while (true) {
        for (int i = 0; i < 4; ++i) {
            const uint32_t color_index = esp_random() % Colors::NUM_COLORS;

            robot.leds.set_color(
                static_cast<Device::Leds::led_id_t>(i),
                Colors::LIST[color_index]
            );
        }

        delay(1000);
    }
}

extern "C" void app_main() {
    constexpr const char* TAG = "Main";

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

    robot.battery.initialize();
    robot.leds.initialize();
    robot.display.initialize();

    robot.set_status_led(true);
    robot.set_bottom_led(false);

    QueueHandle_t log_queue = xQueueCreate(64, sizeof(Logger::log_item));

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

    robot.display.clear();
    robot.display.set_cursor_position(0, 0);
    robot.display.write_text("Hello, World!");

    while (true) {

    }
}