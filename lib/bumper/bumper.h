// BUMPER_H
// Boboter
// (C) MarioS271 2025

#include <cstdint>
#include "driver/gpio.h"

enum bumper_num_t : uint8_t {
    BUMPER_LEFT = 0,
    BUMPER_RIGHT = 1
};

class Bumper {
private:
    static constexpr gpio_num_t LEFT_BUMPER_PIN = GPIO_NUM_35;
    static constexpr gpio_num_t RIGHT_BUMPER_PIN = GPIO_NUM_34;

    bumper_num_t bumper_num;
    gpio_num_t bumper_pin;
    bool error;

public:
    explicit Bumper(bumper_num_t bumper_number);

    bool isHit() const;
};