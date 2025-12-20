/**
 * @file line_follow_task.cpp
 *
 * @authors MarioS271, ThomS263
 * @copyright MIT License
*/

#include "tasks/line_follow/line_follow_task.hpp"

void Boboter::Task::LineFollow::Task(void* params) {
    using namespace Constants;
    using namespace Boboter::Helpers::Colors;
    using namespace Boboter::Types::RGB_Color;

    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ioShieldFlex = ctx->ioShieldFlex;
    Leds &leds = ctx->leds;
    BatteryManager &batteryManager = ctx->batteryManager;

    rgb_color_t colorArray[8] = { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE };

    // Animation step counters
    uint8_t step_random = 0;
    uint8_t step_breath = 0;
    uint8_t step_police = 0;
    uint8_t step_warning = 0;
    uint8_t step_battery = 0;
    uint8_t step_strobe = 0; // For non-blocking strobe

    // Last update timestamps (µs)
    uint64_t last_random = 0;
    uint64_t last_breath = 0;
    uint64_t last_police = 0;
    uint64_t last_warning = 0;
    uint64_t last_battery = 0;
    uint64_t last_strobe = 0;

    // Intervals
    const uint64_t INTERVAL_RANDOM   = 300000;    // 300 ms
    const uint64_t INTERVAL_BREATH   = 10000;     // 10 ms
    const uint64_t INTERVAL_POLICE   = 200000;    // 200 ms
    const uint64_t INTERVAL_WARNING_ON  = 500000; // 500 ms ON
    const uint64_t INTERVAL_WARNING_OFF = 500000; // 500 ms OFF
    const uint64_t INTERVAL_BATTERY = 1000000;    // 1 s battery update

    // Thresholds for battery percentage
    const uint8_t P_HIGH   = 80;
    const uint8_t P_MEDIUM = 50;
    const uint8_t P_LOW    = 20;

    // Strobe constants
    const uint64_t STROBE_PULSE = 40000; // 40 ms
    const uint64_t GROUP_PAUSE = 400000; // 400 ms

    while (true) {
        uint8_t mode = ioShieldFlex.get<int>("leds_mode");
        uint64_t now = esp_timer_get_time();

        switch (mode) {
            // 0: LEDs OFF
            case 0:
                leds.setAll(OFF);
                break;

            // 1: RANDOM BLINK
            case 1:
                if (now - last_random >= INTERVAL_RANDOM) {
                    last_random = now;
                    leds.setColor(LED_FRONT_LEFT, colorArray[esp_random() % 8]);
                    leds.setColor(LED_FRONT_RIGHT, colorArray[esp_random() % 8]);
                    leds.setColor(LED_BACK_LEFT, colorArray[esp_random() % 8]);
                    leds.setColor(LED_BACK_RIGHT, colorArray[esp_random() % 8]);
                }
                break;

            // 2: BREATHING
            case 2:
                if (now - last_breath >= INTERVAL_BREATH) {
                    last_breath = now;
                    uint8_t fade = (step_breath < 128) ? step_breath * 2 : (255 - (step_breath - 128) * 2);
                    rgb_color_t c = rgb_color_t{fade, fade, fade};
                    leds.setAll(c);
                    step_breath = (step_breath + 1) % 256;
                }
                break;

            // 3: POLICE FLASH
            case 3:
                if (now - last_breath >= INTERVAL_BREATH) {
                    last_police = now;
                    bool leftBlue = (step_police % 2 == 0);
                    bool rightRed = (step_police % 2 == 1);

                    leds.setColor(LED_FRONT_LEFT, leftBlue  ? BLUE : OFF);
                    leds.setColor(LED_BACK_LEFT, leftBlue  ? BLUE : OFF);
                    leds.setColor(LED_FRONT_RIGHT, rightRed  ? RED  : OFF);
                    leds.setColor(LED_BACK_RIGHT, rightRed  ? RED  : OFF);
                    step_police++;
                }
                break;

            // 4: WARNING FLASHER
            case 4: {
                    bool warning_on = (step_warning % 2 == 0);
                    if ((warning_on && now - last_warning >= INTERVAL_WARNING_ON) ||
                        (!warning_on && now - last_warning >= INTERVAL_WARNING_OFF))
                    {
                        last_warning = now;
                        leds.setAll(warning_on ? YELLOW : OFF);
                        step_warning++;
                    }
                }
                break;

            // 5: BATTERY STATUS
            case 5:
                if (now - last_breath >= INTERVAL_BREATH) {
                    last_battery = now;
                    uint8_t percentage = batteryManager.getPercentage();
                    rgb_color_t color = OFF;
                    bool blink = false;

                    if (percentage >= P_HIGH)
                        color = GREEN;
                    else if (percentage >= P_MEDIUM)
                        color = YELLOW;
                    else if (percentage >= P_LOW)
                        color = RED;
                    else {
                        color = RED;
                        blink = true;
                    }

                    if (blink) {
                        bool on = (step_battery % 2 == 0);
                        leds.setAll(on ? color : OFF);
                        step_battery++;
                    }
                    else
                        leds.setAll(color);
                }
                break;

            // 6: CAR LIGHTS
            case 6:
                leds.setColor(LED_FRONT_LEFT, WHITE);
                leds.setColor(LED_FRONT_RIGHT, WHITE);
                leds.setColor(LED_BACK_LEFT, RED);
                leds.setColor(LED_BACK_RIGHT, RED);
                break;

            // 7: NON-BLOCKING FRONT/BACK STROBE
            case 7:
                if (now - last_breath >= INTERVAL_BREATH) {
                    last_strobe = now;
                    bool isFrontPhase = (step_strobe < 8);
                    bool ledOn = (step_strobe % 2 == 0);

                    // Determine which LEDs to flash
                    if (isFrontPhase) {
                        leds.setColor(LED_FRONT_LEFT, ledOn ? BLUE : OFF);
                        leds.setColor(LED_FRONT_RIGHT, ledOn ? BLUE : OFF);
                    }
                    else {
                        leds.setColor(LED_BACK_LEFT, ledOn ? BLUE : OFF);
                        leds.setColor(LED_BACK_RIGHT, ledOn ? BLUE : OFF);
                    }

                    // Step counter update
                    step_strobe++;
                    if (step_strobe >= 16) {  // 8 steps front + 8 steps back
                        step_strobe = 0;
                        last_strobe += GROUP_PAUSE; // Add group pause before restarting
                    }
                }
                break;
        }

        delay(1);
    }
}