/**
 * @file main.cpp
 * @authors MarioS271
*/

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <source_location>
#include <esp_random.h>
#include <freertos/FreeRTOS.h>

#include "include/flags.hpp"
#include "include/constants.hpp"

#include "types/flex_struct.hpp"
#include "types/system_context.hpp"

#include "helpers/i2c_utils.hpp"
#include "helpers/adc_utils.hpp"

#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"
#include "lib/battery/battery.hpp"
#include "lib/leds/leds.hpp"
#include "lib/motors/motors.hpp"
#include "lib/encoder/encoder.hpp"
#include "lib/bumper/bumper.hpp"
#include "lib/ultrasonic/ultrasonic.hpp"
#include "lib/gyro/gyro.hpp"
#include "lib/colorsensor/colorsensor.hpp"
#include "lib/linefollower/linefollower.hpp"
#include "lib/move/move.hpp"
#include "lib/io_shield/io_shield.hpp"
#include "lib/web_ui/web_ui.hpp"

#include "tasks/sensor_test.hpp"
#include "tasks/led_task.hpp"
#include "tasks/io_shield_task.hpp"
#include "tasks/web_ui_task.hpp"
#include "tasks/line_follow_task.hpp"


extern "C" void app_main() {
    using namespace Main;

    delay(500);

    LOGI(TAG, " === BOBOTER is starting ===");
    LOGI(TAG, "Hello, World!");

    esp_log_level_set("gpio", ESP_LOG_NONE);

    init_adc();

    init_i2c();
    scan_i2c_addresses();

    // ERROR_CHECK(TAG, gpio_reset_pin(GPIO_NUM_13));
    // ERROR_CHECK(TAG, gpio_set_direction(GPIO_NUM_13, GPIO_MODE_OUTPUT));
    // ERROR_CHECK(TAG, gpio_set_level(GPIO_NUM_13, 1));

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