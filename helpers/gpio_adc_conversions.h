/**
 * @file gpio_adc_conversions.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <optional>
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>

/**
 * @brief Converts a @c gpio_num_t to an @c adc_channel_t for ADC1
 *
 * @return The converted ADC channel, or @c std::nullopt if the given pin was invalid
 *
 * @param gpio_pin The GPIO pin to convert
 */
inline std::optional<adc_channel_t> gpio_to_adc_channel(const gpio_num_t gpio_pin) {
    switch (gpio_pin) {
        case GPIO_NUM_36: return ADC_CHANNEL_0;
        case GPIO_NUM_39: return ADC_CHANNEL_3;
        case GPIO_NUM_32: return ADC_CHANNEL_4;
        case GPIO_NUM_33: return ADC_CHANNEL_5;
        case GPIO_NUM_34: return ADC_CHANNEL_6;
        case GPIO_NUM_35: return ADC_CHANNEL_7;
        default: return std::nullopt;
    }
}

/**
 * @brief Converts an @c adc_channel_t to a @c gpio_num_t for ADC1
 *
 * @return The converted GPIO pin, or @c std::nullopt if the given channel was invalid
 *
 * @param adc_channel The ADC channel to convert
 */
inline std::optional<gpio_num_t> adc_channel_to_gpio(const adc_channel_t adc_channel) {
    switch (adc_channel) {
        case ADC_CHANNEL_0: return GPIO_NUM_36;
        case ADC_CHANNEL_3: return GPIO_NUM_39;
        case ADC_CHANNEL_4: return GPIO_NUM_32;
        case ADC_CHANNEL_5: return GPIO_NUM_33;
        case ADC_CHANNEL_6: return GPIO_NUM_34;
        case ADC_CHANNEL_7: return GPIO_NUM_35;
        default: return std::nullopt;
    }
}