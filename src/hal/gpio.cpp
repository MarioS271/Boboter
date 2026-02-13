/**
 * @file gpio.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "include/hal/gpio.h"

#include <algorithm>
#include "types/smart_mutex.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace HAL::GPIO {
    Controller::Controller() :
        mutex(xSemaphoreCreateMutex())
    {
        LOGD("Constructor called");
    }

    Controller::~Controller() {
        LOGD("Destructor called");

        shutdown();

        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
            mutex = nullptr;
        }
    }

    void Controller::shutdown() {
        smart_mutex lock(mutex);

        for (const auto& entry : registered_entries) {
            gpio_reset_pin(entry.gpio_pin);
        }

        registered_entries.clear();

        LOGI("Shut down HAL::GPIO::Controller");
    }

    void Controller::add(const pin_config_t& entry) {
        smart_mutex lock(mutex);

        const auto pin_as_int = static_cast<uint8_t>(entry.gpio_pin);

        gpio_pullup_t pullup_mode = GPIO_PULLUP_DISABLE;
        gpio_pulldown_t pulldown_mode = GPIO_PULLDOWN_DISABLE;

        switch (entry.pull_mode) {
            case GPIO_PULLUP_ONLY:
                pullup_mode = GPIO_PULLUP_ENABLE;
                break;

            case GPIO_PULLDOWN_ONLY:
                pulldown_mode = GPIO_PULLDOWN_ENABLE;
                break;

            case GPIO_PULLUP_PULLDOWN:
                pullup_mode = GPIO_PULLUP_ENABLE;
                pulldown_mode = GPIO_PULLDOWN_ENABLE;
                break;

            default: break;
        }

        ERROR_CHECK(gpio_hold_dis(entry.gpio_pin));

        const gpio_config_t config = {
            .pin_bit_mask = (1ULL << pin_as_int),
            .mode = entry.mode,
            .pull_up_en = pullup_mode,
            .pull_down_en = pulldown_mode,
            .intr_type = entry.intr_type
        };
        ERROR_CHECK(gpio_config(&config));

        registered_entries.push_back(
            saved_config_entry_t{
                entry.gpio_pin,
                entry.mode
            }
        );

        LOGD("Initialized pin %d", pin_as_int);
    }

    void Controller::set_level(const gpio_num_t gpio_pin, const level_t level) const {
        smart_mutex lock(mutex);

        const auto entry = std::ranges::find_if(
            registered_entries,
            [gpio_pin](const gpio_num_t pin) {
                return pin == gpio_pin;
            },
            &saved_config_entry_t::gpio_pin
        );

        if (entry != registered_entries.end() && entry->mode & GPIO_MODE_DEF_OUTPUT) {
            WARN_CHECK(gpio_set_level(gpio_pin, static_cast<uint32_t>(level)));
        } else {
            LOGE("Unable to set GPIO pin level, invalid pin");
            abort();
        }
    }

    level_t Controller::get_level(const gpio_num_t gpio_pin) const {
        smart_mutex lock(mutex);

        const auto entry = std::ranges::find_if(
            registered_entries,
            [gpio_pin](const gpio_num_t pin) {
                return pin == gpio_pin;
            },
            &saved_config_entry_t::gpio_pin
        );

        if (entry != registered_entries.end() && entry->mode & GPIO_MODE_DEF_INPUT) {
            return static_cast<level_t>(gpio_get_level(gpio_pin));
        }

        LOGE("Unable to get GPIO pin level, invalid pin");
        abort();
    }
}