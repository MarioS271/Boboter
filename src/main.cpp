/**
 * @file main.cpp
 * @authors MarioS271
 */

// Official Libraries
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <esp_random.h>
#include <freertos/FreeRTOS.h>

// Headers
#include "flags.hpp"
#include "constants.hpp"

// Types
#include "flex_struct.hpp"
#include "system_context.hpp"

// Helpers
#include "i2c_utils.hpp"
#include "adc_utils.hpp"

// Custom Libraries
#include "logger.hpp"
#include "error.hpp"
#include "battery.hpp"
#include "leds.hpp"
#include "motors.hpp"
#include "encoder.hpp"
#include "bumper.hpp"
#include "ultrasonic.hpp"
#include "gyro.hpp"
#include "colorsensor.hpp"
#include "linefollower.hpp"
#include "move.hpp"
#include "io_shield.hpp"
#include "web_ui.hpp"

// Tasks
#include "sensor_test.hpp"
#include "led_task.hpp"
#include "io_shield_task.hpp"
#include "web_ui_task.hpp"
#include "line_follow_task.hpp"


extern "C" void app_main() {
    using namespace Main;

    delay(500);

    LOGI(TAG, " === BOBOTER is starting ===");
    LOGI(TAG, "Hello, World!");

    esp_log_level_set("gpio", ESP_LOG_NONE);

    init_adc();

    init_i2c();
    scan_i2c_addresses();

    ERROR_CHECK(TAG, gpio_reset_pin(GPIO_NUM_13));
    ERROR_CHECK(TAG, gpio_set_direction(GPIO_NUM_13, GPIO_MODE_OUTPUT));
    ERROR_CHECK(TAG, gpio_set_level(GPIO_NUM_13, 1));

    static BatteryManager batteryManager = BatteryManager();
    static Leds leds = Leds();
    static Motor motorL = Motor(MOTOR_LEFT);
    static Motor motorR = Motor(MOTOR_RIGHT);
    static Encoder encoderL = Encoder(ENCODER_LEFT);
    static Encoder encoderR = Encoder(ENCODER_RIGHT);
    static Bumper bumperL = Bumper(BUMPER_LEFT);
    static Bumper bumperR = Bumper(BUMPER_RIGHT);
    static Ultrasonic usonic = Ultrasonic();
    static Gyro gyro = Gyro();
    static Linefollower linefollower = Linefollower();
    static Move move = Move(motorL, motorR, encoderL, encoderR, bumperL, bumperR, usonic);
    static IOShield ioShield = IOShield();
    static Colorsensor colorsensor = Colorsensor();

    LOGI(TAG, "Created all Objects successfully");

    static SystemContext sysctx = {
        batteryManager,
        leds, motorL, motorR,
        encoderL, encoderR, bumperL,
        bumperR, usonic, gyro,
        colorsensor, linefollower, move,
        ioShield,

        FlexStruct(),  // LedsTask
        FlexStruct(),  // IOShieldTask
        FlexStruct(),  // WebUITask
    };

    LOGI(TAG, "Created SystemContext");
    LOGI(TAG, "Starting FreeRTOS Task(s)");
    

    if (ENABLE_SENSOR_TEST_MODE)
    {
        xTaskCreate(sensorTest, "SensorTest", TASK_STACK_DEPTH, &sysctx, SENSOR_TEST_TASK_PRIORITY, nullptr);
        return;
    }

    xTaskCreate(ledTask, "LedTask", TASK_STACK_DEPTH, &sysctx, LED_TASK_PRIORITY, nullptr);
    if (ENABLE_WEBUI)
        xTaskCreate(webuiTask, "WebUITask", TASK_STACK_DEPTH, &sysctx, WEBUI_TASK_PRIORITY, nullptr);
    if (ENABLE_IO_SHIELD)
        xTaskCreate(ioShieldTask, "IOShieldTask", TASK_STACK_DEPTH, &sysctx, IO_SHIELD_TASK_PRIORITY, nullptr);

    xTaskCreate(lineFollowTask, "LineFollowTask", TASK_STACK_DEPTH, &sysctx, 8, nullptr);

    // rgb_color_t color = { 0, 0, 0 };
    // colorsensor.enable();

    // while (true) {
    //     color = colorsensor.readRGB();
    //     LOGI(TAG, "r: %d, g: %d, b: %d", color.r, color.g, color.b);

    //     delay(500);
    // }
}