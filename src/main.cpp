// MAIN_CPP
// Boboter
// (C) MarioS271 2025

// Official Libraries
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <esp_random.h>
#include <freertos/FreeRTOS.h>

// Headers
#include "flags.h"
#include "constants.h"

// Types
#include "flex_struct.h"
#include "system_context.h"

// Helpers
#include "i2c_utils.h"
#include "adc_utils.h"

// Custom Libraries
#include "logger.h"
#include "error.h"
#include "battery.h"
#include "leds.h"
#include "motors.h"
#include "encoder.h"
#include "bumper.h"
#include "ultrasonic.h"
#include "gyro.h"
#include "colorsensor.h"
#include "linefollower.h"
#include "move.h"
#include "io_shield.h"
#include "web_ui.h"

// Tasks
#include "sensor_test.h"
#include "led_task.h"
#include "io_shield_task.h"
#include "web_ui_task.h"
#include "line_follow_task.h"


extern "C" void app_main() {
    constexpr const char* TAG = "MAIN";

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
    

    if (ENABLE_SENSOR_TEST_MODE) {
        xTaskCreate(sensorTest, "SensorTest", TASK_STACK_DEPTH, &sysctx, SENSOR_TEST_TASK_PRIORITY, nullptr);
        return;
    }

    xTaskCreate(ledTask, "LedTask", TASK_STACK_DEPTH, &sysctx, LED_TASK_PRIORITY, nullptr);
    if (ENABLE_WEBUI) xTaskCreate(webuiTask, "WebUITask", TASK_STACK_DEPTH, &sysctx, WEBUI_TASK_PRIORITY, nullptr);
    if (ENABLE_IO_SHIELD) xTaskCreate(ioShieldTask, "IOShieldTask", TASK_STACK_DEPTH, &sysctx, IO_SHIELD_TASK_PRIORITY, nullptr);

    xTaskCreate(lineFollowTask, "LineFollowTask", TASK_STACK_DEPTH, &sysctx, 8, nullptr);

    // rgb_color_t color = { 0, 0, 0 };
    // colorsensor.enable();

    // while (true) {
    //     color = colorsensor.readRGB();
    //     LOGI(TAG, "r: %d, g: %d, b: %d", color.r, color.g, color.b);

    //     delay(500);
    // }
}