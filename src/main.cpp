/**
 * @file main.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <string>
#include <vector>
#include <atomic>
#include <algorithm>
#include <unordered_map>
#include <source_location>

#include <esp_timer.h>
#include <esp_random.h>
#include <rom/ets_sys.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/i2c.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <freertos/FreeRTOS.h>

#include "include/flags.hpp"
#include "include/constants.hpp"

#include "types/flex.hpp"
#include "types/system_context.hpp"
#include "types/rgb_color.hpp"
#include "types/note.hpp"
#include "types/sound.hpp"

#include "helpers/map.hpp"
#include "helpers/delay.hpp"
#include "helpers/predef_colors.hpp"
#include "helpers/i2c_utils.hpp"
#include "helpers/adc_utils.hpp"

#include "lib/logger/logger.hpp"
#include "lib/error/error.hpp"
#include "lib/battery/battery.hpp"
#include "lib/rgb_leds/rgb_leds.hpp"
#include "lib/other_leds/other_leds.hpp"
// #include "lib/pid_controller/pid_controller.hpp"
#include "lib/motor/motor.hpp"
#include "lib/encoder/encoder.hpp"
#include "lib/bumper/bumper.hpp"
#include "lib/ultrasonic/ultrasonic.hpp"
#include "lib/gyro/gyro.hpp"
#include "lib/magnetometer/magnetometer.hpp"
// #include "lib/colorsensor/colorsensor.hpp"
#include "lib/linefollower/linefollower.hpp"
#include "lib/display/display.hpp"
#include "lib/buzzer/buzzer.hpp"
#include "lib/button/button.hpp"

#include "tasks/secure_task/secure_task.hpp"
// #include "tasks/system_task/system_task.hpp"
// #include "tasks/pid_task/pid_task.hpp"
// #include "tasks/io_task/io_task.hpp"
// #include "tasks/leds_task/leds_task.hpp"
// #include "tasks/buzzer_task/buzzer_task.hpp"
#include "tasks/sensor_test_task/sensor_test_task.hpp"


extern "C" void app_main() {
    using namespace Boboter::Main;
    using namespace Boboter::Main::Constants;
    using namespace Boboter::Types;
    using namespace Boboter::Libs::Logger;

    Boboter::Helpers::delay(500);

    LOGI(TAG, " === BOBOTER is starting ===");
    LOGI(TAG, "Hello, World!");

    esp_log_level_set("gpio", ESP_LOG_NONE);

    Boboter::Helpers::ADC::init();

    Boboter::Helpers::I2C::init();
    Boboter::Helpers::I2C::scan_adresses();

    static Boboter::Libs::Battery::Battery battery;
    static Boboter::Libs::RGB_Leds::RGB_Leds rgb_leds;
    static Boboter::Libs::OtherLeds::OtherLeds other_leds;
    // static Boboter::Libs::PID_Controller::PID_Controller pid_controller;
    static Boboter::Libs::Motor::Motor motor_left(Boboter::Types::Motor::Id::LEFT, true);
    static Boboter::Libs::Motor::Motor motor_right(Boboter::Types::Motor::Id::RIGHT, false);
    static Boboter::Libs::Encoder::Encoder encoder_left(Boboter::Types::Encoder::Id::LEFT);
    static Boboter::Libs::Encoder::Encoder encoder_right(Boboter::Types::Encoder::Id::RIGHT);
    static Boboter::Libs::Bumper::Bumper bumper_left(Boboter::Types::Bumper::Id::LEFT);
    static Boboter::Libs::Bumper::Bumper bumper_right(Boboter::Types::Bumper::Id::RIGHT);
    static Boboter::Libs::Ultrasonic::Ultrasonic ultrasonic;
    static Boboter::Libs::Gyro::Gyro gyro;
    static Boboter::Libs::Magnetometer::Magnetometer magnetometer;
    // static Boboter::Libs::Colorsensor::Colorsensor colorsensor;
    static Boboter::Libs::Linefollower::Linefollower linefollower;
    static Boboter::Libs::Display::Display display;
    static Boboter::Libs::Buzzer::Buzzer buzzer;
    static Boboter::Libs::Button::Button button_primary(Boboter::Types::Button::Id::PRIMARY);
    static Boboter::Libs::Button::Button button_secondary(Boboter::Types::Button::Id::SECONDARY);

    LOGI(TAG, "Created all Objects successfully");

    static SystemContext sysctx = {
        .battery = battery,
        .rgb_leds = rgb_leds,
        .other_leds = other_leds,
        // .pid_controller = pid_controller,
        .motor_left = motor_left,
        .motor_right = motor_right,
        .encoder_left = encoder_left,
        .encoder_right = encoder_right,
        .bumper_left = bumper_left,
        .bumper_right = bumper_right,
        .ultrasonic = ultrasonic,
        .gyro = gyro,
        .magnetometer = magnetometer,
        // .colorsensor = colorsensor,
        .linefollower = linefollower,
        .display = display,
        .buzzer = buzzer,
        .button_primary = button_primary,
        .button_secondary = button_secondary,
        
        .system_flex = flex(),
        .buzzer_flex = flex(),
    };

    LOGI(TAG, "Created SystemContext");
    LOGI(TAG, "Starting FreeRTOS Tasks");
    
    // Secure Task
    xTaskCreate(
        Boboter::Tasks::Secure::task,
        "SecureTask",
        SECURE_TASK_STACK_DEPTH,
        &sysctx,
        SECURE_TASK_PRIORITY,
        nullptr
    );

    // // System Task
    // xTaskCreate(
    //     Boboter::Tasks::System::task,
    //     "SystemTask",
    //     SYSTEM_TASK_STACK_DEPTH,
    //     &sysctx,
    //     SYSTEM_TASK_PRIORITY,
    //     nullptr
    // );

    // Sensor Test Task
    if (Flags::ENABLE_SENSOR_TEST_MODE) {
        xTaskCreate(
            Boboter::Tasks::SensorTest::task,
            "SensorTestTask",
            SENSOR_TEST_TASK_STACK_DEPTH,
            &sysctx,
            SENSOR_TEST_TASK_PRIORITY,
            nullptr
        );
        vTaskDelete(nullptr);
    }

    // // PID Task
    // xTaskCreate(
    //     Boboter::Tasks::PID::task,
    //     "PID_Task",
    //     PID_TASK_STACK_DEPTH,
    //     &sysctx,
    //     PID_TASK_PRIORITY,
    //     nullptr
    // );

    // // IO Task
    // xTaskCreate(
    //     Boboter::Tasks::IO::task,
    //     "IO_Task",
    //     IO_TASK_STACK_DEPTH,
    //     &sysctx,
    //     IO_TASK_PRIORITY,
    //     nullptr
    // );

    // // LEDs Task
    // xTaskCreate(
    //     Boboter::Tasks::Leds::task,
    //     "LEDs_Task",
    //     LEDS_TASK_STACK_DEPTH,
    //     &sysctx,
    //     LEDS_TASK_PRIORITY,
    //     nullptr
    // );

    // // Buzzer Task
    // xTaskCreate(
    //     Boboter::Tasks::Buzzer::task,
    //     "BuzzerTask",
    //     BUZZER_TASK_STACK_DEPTH,
    //     &sysctx,
    //     BUZZER_TASK_PRIORITY,
    //     nullptr
    // );

    vTaskDelete(nullptr);
}