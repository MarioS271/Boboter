/**
 * @file battery.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "battery.h"

#include "include/robot.h"
#include "helpers/map_value.h"
#include "lib/logger/logger.h"

namespace Device {
    Battery::Battery(Robot& robot) :
        robot(robot),
        percentage(0),
        voltage(0)
    {
        LOGD("Constructor called");
    }

    Battery::~Battery() {
        LOGD("Destructor called");
    }

    void Battery::initialize() {
        robot.adc.add(ADC_CHANNEL);
        LOGI("Initialized Device::Battery");
    }

    void Battery::update() {
        voltage = robot.adc.read_millivolts(ADC_CHANNEL, NUM_SAMPLES) * 147 / 100;

        percentage = static_cast<int8_t>(map_value(voltage, 3300, 4200, 0, 100));
        if (percentage > 100) {
            percentage = 100;
        }

        LOGV("Measured %hhu%% (%f V) battery remaining", percentage, static_cast<float>(voltage) / 1000.0f);
    }
}