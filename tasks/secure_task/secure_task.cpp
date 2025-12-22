/**
 * @file secure_task.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#include "secure_task.hpp"

#include <string>
#include <vector>
#include <esp_timer.h>
#include "types/system_context.hpp"
#include "types/flex.hpp"
#include "types/note.hpp"
#include "helpers/delay.hpp"
#include "helpers/note_frequencies.hpp"
#include "lib/logger/logger.hpp"
#include "lib/battery/battery.hpp"

namespace Boboter::Tasks::Secure {
    void task(void* params) {
        using namespace Config;
        using namespace Internal;
        using Boboter::Types::SystemContext;
        using Boboter::Types::flex;
        using Boboter::Helpers::delay;
        using namespace Boboter::Helpers::Note_Frequencies;
        using namespace Boboter::Libs::Logger;

        SystemContext* ctx = static_cast<SystemContext*>(params);
        Boboter::Libs::Battery::Battery& battery = ctx->battery;
        flex& system_flex = ctx->system_flex;
        flex& buzzer_flex = ctx->buzzer_flex;

        uint16_t battery_voltage_mv = 0;
        uint64_t current_time = 0;
        uint64_t too_low_start_time = 0;
        bool is_low = false;
        bool alert_triggered = false;

        while (true) {
            battery_voltage_mv = battery.get_voltage_mv();
            current_time = esp_timer_get_time();

            if (battery_voltage_mv < BATTERY_MIN_VOLTAGE_MV) {
                if (!is_low) {
                    is_low = true;
                    too_low_start_time = current_time;
                }
            } else {
                is_low = false;
                alert_triggered = false;
            }

            if (is_low && !alert_triggered && current_time - too_low_start_time >= ALERT_TIME_MS * 1000) {
                alert_triggered = true;
                system_flex.set<std::string>("status_led_mode", "flashing_fast");
                buzzer_flex.set<std::vector<Boboter::Types::note>>(
                    "sound_to_play",
                    { {E4, 500}, {E3, 500}, {C4, 500}, {C3, 500} }
                );
            }

            if (is_low && current_time - too_low_start_time >= SHUTOFF_TIME_MS * 1000)
                // abort();
                LOGI(TAG, "expired!");
            delay(10);
        }
    }
}