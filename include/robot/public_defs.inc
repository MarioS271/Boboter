/**
* @file public_defs.inc
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

static constexpr gpio_num_t STATUS_LED_PIN = GPIO_NUM_5;
static constexpr gpio_num_t BOTTOM_LED_PIN = GPIO_NUM_13;

struct task_config_t {
    TaskFunction_t task_function;
    const char* task_name;
    const uint32_t stack_depth;
    void* const params_for_task;
    UBaseType_t priority;
    TaskHandle_t* const created_task_handle;
    const BaseType_t core_id;
};

enum class robot_mode_t : uint8_t {
    NONE = 0,
    APP = 1,
    GAMEPAD = 2
};

enum class status_led_mode_t : uint8_t {
    OFF = 0,
    ON = 1,
    BLINK_SLOW = 2,
    BLINK_FAST = 3,
};

enum class rgb_leds_mode_t : uint8_t {
    OFF = 0,
    HEADLIGHTS = 1,
    RANDOM_COLORS = 2,
    POLICE = 3
};