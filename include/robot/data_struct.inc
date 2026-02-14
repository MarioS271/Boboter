/**
 * @file data_struct.inc
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

struct data_struct {
    robot_mode_t mode;

    struct leds_struct {
        status_led_mode_t status_led_mode;
        rgb_leds_mode_t rgb_leds_mode;
    } leds{};

    struct motors_struct {
        uint16_t target_speed_left;
        uint16_t target_speed_right;
        uint16_t ramp_time_left;
        uint16_t ramp_time_right;
        Device::Motors::motor_direction_t direction_left;
        Device::Motors::motor_direction_t direction_right;
    } motors{};
};
protected_struct<data_struct> data{};