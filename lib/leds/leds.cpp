// LEDS_CPP
// Boboter
// (C) MarioS271 2025

#include "leds.h"

#include "esp_log.h"

static const char* TAG = "class_leds";

// Constructor
Leds::Leds() {
    gpio_config_t gpio_conf = {};
    gpio_conf.pin_bit_mask = (1ULL << MOSI_PIN) | (1ULL << CLK_PIN);
    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_conf);

    ESP_LOGI(TAG, "LEDs initialized on MOSI: %d, CLK: %d", MOSI_PIN, CLK_PIN);

    allOff();
}



// Private Helper: Send Bit Function
void Leds::sendBit(bool bit) {
    gpio_set_level(MOSI_PIN, bit ? 1 : 0);
    gpio_set_level(CLK_PIN, 1);
    gpio_set_level(CLK_PIN, 0);
}

// Private Helper: Send Byte Function
void Leds::sendByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        sendBit(byte & (1 << i));
    }
}

// Private Helper: Send Frame Function
void Leds::sendFrame(hex_color_t color) {
    sendByte(0b11100000 | (31 & 0x1F)); // 5-bit global brightness
    sendByte(color.r);
    sendByte(color.b);
    sendByte(color.g);
}

// Private Helper: Update to LEDs Function
void Leds::update() {
    // Start frame
    for (int i = 0; i < 4; i++) sendByte(0x00);

    // LED frames
    for (int i = 0; i < 4; i++) sendFrame(buffer[i]);

    // End frame: enough 1s/zeros to latch
    for (int i = 0; i < 4; i++) sendByte(0xFF);
}



// Set Single Color Function
void Leds::setColor(led_pos_t led_pos, hex_color_t color) {
    if (led_pos >= 4) {
        ESP_LOGW(TAG, "Invalid LED position: %d", led_pos);
        return;
    }
    buffer[led_pos] = color;
    ESP_LOGI(TAG, "LED %d set to R=%d G=%d B=%d", led_pos, color.r, color.g, color.b);
    update();
}

// Set All Colors Function
void Leds::setAll(hex_color_t color) {
    for (int i = 0; i < 4; i++) buffer[i] = color;
    ESP_LOGI(TAG, "All LEDs set to R=%d G=%d B=%d", color.r, color.g, color.b);
    update();
}

// Turn Single Off Function
void Leds::setOff(led_pos_t led_pos) {
    ESP_LOGI(TAG, "Turning LED %d off", led_pos);
    setColor(led_pos, {0, 0, 0});
}

// Turn All Off Function
void Leds::allOff() {
    ESP_LOGI(TAG, "Turning all LEDs off");
    setAll({0, 0, 0});
}