# Tasks

The firmware is segmented into a few FreeRTOS tasks to separate and categorize the individual workloads.

Cores 0 and 1 are both used. While Core 0 is used for low-level system functionality and running the main modes,
Core 1 is used for more general, higher level stuff such as I/O, PID, motor ramping and more.

Here is an overview of all tasks:

| Task Name        | Priority | Stack Depth | Assigned Core | On what condition is it created                    |
|------------------|----------|-------------|---------------|----------------------------------------------------|
| Secure Task      | 24       | 2048        | Core 0        | Is always created                                  |
| PID Task         | 19       | 4096        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Ramp Task        | 18       | 2048        | Core 1        | Is always created                                  |
| IO Task          | 15       | 4096        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| App Backend Task | 12       | 8192        | Core 0        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Gamepad Task     | 11       | 8192        | Core 0        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Log Task         | 7        | 4096        | Core 1        | Is always created                                  |
| Status LED Task  | 5        | 2048        | Core 1        | Is always created                                  |
| RGB LEDs Task    | 3        | 2048        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Buzzer Task      | 1        | 2048        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Test Task        | 20       | 2048        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **true**  |


### Secure Task
The secure task is for making sure that the system stays in good health,
by for example making sure to prevent battery over-discharge.

### PID Task
This task performs continuous PID calculations to minimize errors. It ensures the robot
maintains a stable speed even under load or decreasing battery voltage. This task
directly controls the `robot.motor` object which controls the hardware output to the motor driver.

### Ramp Task
Responsible for smooth motor acceleration and deceleration. It gradually adjusts towards the
target speed to prevent excessive acceleration and robot tipping.

### IO Task
Manages user interaction via the SSD1306 OLED display and two buttons.

### App Backend Task 
Responsible for communicating with the **Boboter App**. It sends data and receives
commands for the robot to execute.
Only runs if `robot.data->mode` is set to `APP`.

### Gamepad Task
Responsible for communicating with a bluetooth gamepad to control the robot.
Only runs if `robot.data->mode` is set to `GAMEPAD`.

### LEDs Task
Manages the NeoPixel RGB LEDs and executes lighting routines.

### Buzzer Task
This task is responsible for sending the buzzer signals and playing sounds or sound
sequences.

### Test Task
This is a special task, which is only started if the flag `ENABLE_TEST_MODE` is set.
It goes over the robot's devices (sensors, motors, ...), and tests them to make sure there are no
hardware defects.