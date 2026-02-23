# Tasks

The firmware is segmented into various FreeRTOS tasks, each with specific responsibilities, to efficiently
manage and categorize workloads. For lighter, cooperative routines, refer to [subtasks.md](subtasks.md).

Cores 0 and 1 are both utilized. Core 0 is generally allocated for critical system functionality and higher-priority
tasks, while Core 1 handles more general processing, communication, and management tasks.

## Overview of FreeRTOS Tasks

| Task Name              | Priority | Stack Size | Assigned Core | On what condition is it created                    |
|------------------------|----------|------------|---------------|----------------------------------------------------|
| Secure Task            | 24       | 2048       | Core 0        | Is always created                                  |
| Drive Task             | 20       | 4096       | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Ramp Task              | 19       | 2048       | Core 1        | Is always created                                  |
| Sensor Reader Task     | 17       | 4096       | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Action Task            | 14       | 4096       | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| App Backend Task       | 12       | 8192       | Core 0        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Gamepad Task           | 11       | 8192       | Core 0        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Log Task               | 9        | 4096       | Core 1        | Is always created                                  |
| Subtask Scheduler Task | 7        | 4096       | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Test Task              | 21       | 2048       | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **true**  |

### Secure Task
This critical task ensures system integrity and prevents hazardous conditions, such as battery over-discharge,
by continuously monitoring vital parameters and initiating safety procedures.

### Drive Task
Manages the robot's motors with built-in corrections (PID controller) which utilize the encoder, and, if present,
the gyroscope, accelerometer and magnetometer.
Performs continuous PID calculations to minimize control errors, ensuring the robot maintains stable speed and
trajectory under varying loads. This task directly controls motor output.

### Ramp Task
Manages smooth acceleration and deceleration of motors, gradually adjusting towards target speeds.

### Sensor Reader Task
Responsible for performing resource-heavy or blocking sensor readings.
It makes raw sensor data available for other tasks.

### Action Task
Executes complex robot behaviors and high-level logic, such as line following, obstacle avoidance, or navigation.
This task interprets commands and translates them into sequences of robot actions.

### App Backend Task
Handles communication with the companion application. It sends telemetry data and receives control commands.

### Gamepad Task
Manages and processes communication with a bluetooth gamepad for robot control via **Bluepad32**.

### Log Task
Processes and outputs system log messages.

### Subtask Scheduler Task
This task manages the execution of multiple lightweight, cooperative subtasks.
It dispatches subtasks based on their requested timing, sharing its execution context to reduce overall RAM usage.
See [subtasks.md](subtasks.md) for details on the subtasks it manages.

### Test Task
A special diagnostic task only activated when `ENABLE_TEST_MODE` is true.
It performs comprehensive checks on the robot's hardware components (sensors, motors, etc.) to
verify functionality and detect defects.
