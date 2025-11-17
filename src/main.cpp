// MAIN_CPP
// Boboter
// (C) MarioS271 2025

// Official Libraries
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "esp_random.h"

// Headers
#include "flags.h"
#include "constants.h"

// Types
#include "flex_struct.h"
#include "system_context.h"

// Helpers
#include "i2c_utils.h"

// Custom Libraries
#include "logger.h"
#include "battery.h"
#include "leds.h"
#include "motors.h"
#include "encoder.h"
#include "bumper.h"
#include "ultrasonic.h"
#include "gyro.h"
#include "linefollower.h"
#include "move.h"
#include "io_shield.h"
#include "web_ui.h"

// Tasks
#include "sensor_test.h"
#include "led_task.h"
#include "io_shield_task.h"
#include "web_ui_task.h"

#define TAG "MAIN"

extern "C" void app_main() {
    LOGI(TAG, " === BOBOTER is starting ===");
    LOGI(TAG, "Hello, World!");

    init_i2c();
    scan_i2c_addresses(TAG);

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
    static Move move = Move(motorL, motorR, encoderL, encoderR, bumperL, bumperR, usonic);
    static IOShield ioShield = IOShield();

    LOGI(TAG, "Created all Objects successfully");

    static SystemContext sysctx = {
        batteryManager,
        
        leds, motorL, motorR,
        encoderL, encoderR, bumperL,
        bumperR, usonic, gyro,
        move, ioShield,

        FlexStruct(),  // LedsTask
        FlexStruct(),  // IOShieldTask
        FlexStruct(),  // WebUITask
    };

    LOGI(TAG, "Created SystemContext");
    LOGI(TAG, "Starting FreeRTOS Task(s)");
    delay(500);


    if (ENABLE_SENSOR_TEST_MODE) { xTaskCreate(sensorTest, "SensorTest", TASK_STACK_DEPTH, &sysctx, 9, nullptr); return; }

    xTaskCreate(ledTask, "LedTask", TASK_STACK_DEPTH, &sysctx, 1, nullptr);
    if (ENABLE_WEBUI) { xTaskCreate(webuiTask, "WebUITask", TASK_STACK_DEPTH, &sysctx, 2, nullptr); }
    if (ENABLE_IO_SHIELD) { xTaskCreate(ioShieldTask, "IOShieldTask", TASK_STACK_DEPTH, &sysctx, 3, nullptr); }
}