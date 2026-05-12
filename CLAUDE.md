# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Commands

| Command             | Purpose                                      |
|---------------------|----------------------------------------------|
| `pio run`           | Build the ESP32 firmware binary              |
| `pio run -t upload` | Build and upload firmware to the board       |
| `pio run -t monitor`| Open serial monitor                          |

Target board: `lolin32` (ESP32). Framework: ESP-IDF via PlatformIO. C++23 standard (`-std=gnu++23`).

## Architecture Overview

### Singleton Robot Object

`Robot` (`include/robot.h`, `src/robot.cpp`) is a singleton holding every subsystem as a public member. It owns references to the four HAL controllers and direct instances of all device drivers. `app_main` in `src/main.cpp` calls `Robot::get_instance()`, configures the HALs, initializes all devices, then spawns FreeRTOS tasks before deleting itself.

### Layer Structure

```
HAL (include/hal/, src/hal/)        — GPIO, LEDC, ADC, I2C controllers (singletons)
  └── Device drivers (devices/)     — Battery, Leds, Display, Buttons, Buzzer,
                                       Motors, Bumpers, Encoders, Ultrasonic, IMU,
                                       Colorsensor, Linefollower
        └── FreeRTOS Tasks (tasks/) — Consume device drivers via Robot singleton
              └── Subtasks (tasks/subtasks/) — Cooperative routines within SubtaskSchedulerTask
```

Each HAL controller is also a singleton (`get_instance()`). Device drivers receive their HAL handles at initialization.

### FreeRTOS Tasks

All tasks run continuously and access the `Robot` singleton. The shared mutable state lives in `robot.data`, a `protected_struct<data_struct>` (see `types/protected_struct.h`) that wraps a FreeRTOS mutex with RAII semantics.

**Task roster (see `docs/tasks.md` for full details):**

| Task                  | Priority | Core | Condition                    |
|-----------------------|----------|------|------------------------------|
| Secure Task           | 24       | 0    | Always                       |
| Drive Task (PID)      | 20       | 1    | `ENABLE_TEST_MODE == false`  |
| Ramp Task             | 19       | 1    | Always                       |
| Sensor Reader Task    | 16       | 1    | `ENABLE_TEST_MODE == false`  |
| Action Task           | 14       | 1    | `ENABLE_TEST_MODE == false`  |
| App Backend Task      | 12       | 0    | `ENABLE_TEST_MODE == false`  |
| Gamepad Task          | 11       | 0    | `ENABLE_TEST_MODE == false`  |
| Log Task              | 9        | 1    | Always                       |
| Subtask Scheduler Task| 7        | 1    | Always                       |
| Test Task             | 21       | 1    | `ENABLE_TEST_MODE == true`   |

### Subtask Scheduler

`tasks/subtask_scheduler_task.cpp` runs lightweight cooperative routines (subtasks in `tasks/subtasks/`) to share one task's stack instead of spawning separate FreeRTOS tasks. Each subtask returns its requested re-run delay. Active subtasks depend on flags (see `docs/subtasks.md`).

### Compile-Time Configuration (`include/flags.h`)

All behavioral switches are `constexpr` booleans/values in `namespace Flags`:
- `ENABLE_TEST_MODE` — replaces normal tasks with `test_task`
- `ENABLE_DISPLAY` / `ENABLE_BUZZER` / `ENABLE_IMU` — conditionally initialize hardware
- `LOWEST_LOG_LEVEL` — minimum log level compiled in
- `ENABLED_VERBOSE_LOG_SOURCES` — bitmask of `log_source` values for verbose output

### Logging

Use the macros `LOGV`, `LOGD`, `LOGI`, `LOGW`, `LOGE` (defined in `lib/logger/logger.h`). Each requires `TAG` (a `const char*`) and `LOG_SOURCE` (a `log_source` enum value) to be defined in the enclosing scope. Before FreeRTOS tasks start, the logger writes directly to serial; after `switch_to_queue_logging()`, messages are queued and processed by `LogTask`.

### Error Handling

- `ERROR_CHECK(expr)` — wraps an `esp_err_t` expression; aborts on failure (requires `TAG` in scope)
- `WARN_CHECK(expr)` — same but only logs a warning, continues execution
- Programming errors (bad config, wrong pin): abort immediately
- Runtime errors (bad sensor data): log warning, substitute zero/approximated value
- Runtime exceptions (critical init failure): abort immediately

See `docs/error_handling.md` for the full policy.

### Thread-Safe Shared State

`protected_struct<T>` (`types/protected_struct.h`) provides mutex-guarded access to `Robot::data`. Two access patterns:
- `robot.data->field = x;` — takes the mutex for the single expression
- `auto lock = robot.data.lock(); lock->field = x;` — holds the mutex across multiple operations; call `lock.unlock()` to release early

The struct detects recursive locking and aborts to prevent deadlocks.

### Custom Types (`types/`)

- `protected_struct<T>` — mutex-guarded struct wrapper
- `smart_mutex` — RAII mutex helper
- `rgb_color`, `vector3`, `quaternion` — value types used by devices
- `note`, `sound` — used by the buzzer device
