/**
 * @file sensor_test_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "sensor_test_task.hpp"

#include "types/flex.hpp"
#include "types/system_context.hpp"
#include "helpers/delay.hpp"
#include "helpers/predef_colors.hpp"

namespace Boboter::Tasks::SensorTest {
    void task(void* params) {
        using namespace Config;
        using namespace Internal;
        using namespace Boboter::Types;
        using Boboter::Helpers::delay;
        using namespace Boboter::Helpers::Colors;
        using namespace Boboter::Libs::Logger;

        SystemContext* ctx = static_cast<SystemContext*>(params);
        Boboter::Libs::RGB_Leds::RGB_Leds& rgb_leds = ctx->rgb_leds;
        Boboter::Libs::Motor::Motor& motor_left = ctx->motor_left;
        Boboter::Libs::Motor::Motor& motor_right = ctx->motor_right;
        Boboter::Libs::Bumper::Bumper& bumper_left = ctx->bumper_left;
        Boboter::Libs::Bumper::Bumper& bumper_right = ctx->bumper_right;
        Boboter::Libs::Ultrasonic::Ultrasonic& ultrasonic = ctx->ultrasonic;

        LOGI(TAG, "Readied SystemContext and Object References");

        // 1) Iterate through all predefined colors to test the LEDs
        rgb_color_t colorArray[NUM_COLORS] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };
        const char* colorNames[NUM_COLORS] = { "RED", "ORANGE", "YELLOW", "GREEN", "CYAN", "BLUE", "MAGENTA", "WHITE"};
        for (int i = 0; i < NUM_COLORS; i++) {
            rgb_leds.set_all(colorArray[i]);
            LOGI(TAG, "Set all LEDs to %s", colorNames[i]);
            delay(500);
        }
        delay(2000);

        // 2) Test Motors
        for (int i = 0; i < 2; i++) {
            motor_left.set_direction(static_cast<Motor::Direction>(i));
            motor_right.set_direction(static_cast<Motor::Direction>(i));

            motor_left.set_speed(Boboter::Libs::Motor::Constants::MAX_MOTOR_SPEED);
            motor_right.set_speed(Boboter::Libs::Motor::Constants::MAX_MOTOR_SPEED);

            LOGI(TAG, "Started both Motors %s with speed MAX_MOTOR_SPEED (%d)",
                i == 0 ? "FORWARD" : "BACKWARD", Boboter::Libs::Motor::Constants::MAX_MOTOR_SPEED);
            delay(2000);

            motor_left.stop(false);
            motor_right.stop();

            LOGI(TAG, "Stopped both Motors");
            delay(1000);
        }
        delay(1000);

        // 3) Sensor Testing Mode
        rgb_leds.all_off();
        while (true) {
            if (bumper_left.is_hit()) {
                rgb_leds.set_color(RGB_Leds::Id::FRONT_LEFT, WHITE);
                LOGI(TAG, "Bumper_Left has hit something!");
            }
            else
                rgb_leds.set_off(RGB_Leds::Id::FRONT_LEFT);

            if (bumper_right.is_hit()) {
                rgb_leds.set_color(RGB_Leds::Id::FRONT_RIGHT, WHITE);
                LOGI(TAG, "Bumper_Right has hit something!");
            }
            else
                rgb_leds.set_off(RGB_Leds::Id::FRONT_RIGHT);

            float distance = ultrasonic.measure_cm();
            if (distance > MAX_DISPLAY_DISTANCE || distance <= 0) {
                rgb_leds.set_color(RGB_Leds::Id::BACK_LEFT, BLUE);
                rgb_leds.set_color(RGB_Leds::Id::BACK_RIGHT, BLUE);
            }
            else {
                LOGI(TAG, "Ultrasonic Sensor Reading: %.2f", distance);

                float t = (distance - MIN_DISPLAY_DISTANCE) / (MAX_DISPLAY_DISTANCE - MIN_DISPLAY_DISTANCE);
                t = t > 1.0 ? 1.0 : (t < 0 ? 0 : t);

                rgb_color_t color;
                if (t < 0.33) {
                    float subT = t / 0.33;
                    color.r = 255;
                    color.g = (uint8_t)(255 * subT);
                    color.b = 0;
                }
                else if (t < 0.66) {
                    float subT = (t - 0.33) / 0.33;
                    color.r = (uint8_t)(255 * (1.0 - subT));
                    color.g = 255;
                    color.b = 0;
                }
                else {
                    float subT = (t - 0.66) / 0.34;
                    color.r = 0;
                    color.g = (uint8_t)(255 * (1.0 - subT));
                    color.b = (uint8_t)(255 * subT);
                }

                rgb_leds.set_color(RGB_Leds::Id::BACK_LEFT, color);
                rgb_leds.set_color(RGB_Leds::Id::BACK_RIGHT, color);
            }
            
            delay(200);
        }
    }
}