# Tasks

The firmware is segmented into a few FreeRTOS tasks to seperate and categorize the individual workloads.

Cores 0 and 1 are both used. While Core 0 is used for low-level system functionality and some outside tasks/libs,
Core 1 is used for higher level stuff such as I/O, PID, motor ramping, ...

Here is an overview of all tasks:

| Task Name   | Priority | Stack Depth | Assigned Core | On what condition is it created                    |
|-------------|----------|-------------|---------------|----------------------------------------------------|
| Secure Task | 24       | 2048        | Core 0        | Is always created                                  |
| System Task | 20       | 2048        | Core 0        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| PID Task    | 15       | 4096        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Ramp Task   | 14       | 2048        | Core 1        | Is always created                                  |
| IO Task     | 10       | 4096        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Log Task    | 6        | 4096        | Core 1        | Is always created                                  |
| LEDs Task   | 3        | 1024        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Buzzer Task | 1        | 1024        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **false** |
| Test Task   | 22       | 4096        | Core 1        | If the flag `ENABLE_TEST_MODE` is set to **true**  |


### Secure Task
The secure task is for making sure that the system stays in good health,
by for example making sure to prevent battery over-discharge.

### System Task
This task handles stuff like system LEDs and other stuff for which there is no specific task.

### PID Task
This task performs continuous PID calculations to minimize errors. It ensures the robot
maintains a stable speed even under load or decreasing battery voltage. This task
directly controls the PWM output to the motors.

### IO Task
Manages user interaction via the SSD1306 OLED display and two buttons.

### LEDs Task
Manages the NeoPixel RGB LEDs and executes lighting routines.

### Buzzer Task
This task is responsible for sending the buzzer signals and playing sounds or sound
sequences.

### Ramp Task
Responsible for smooth motor acceleration and deceleration. It gradually adjusts the
target speed to prevent excessive acceleration and robot tipping.
One ramp task is created for each motor that gets instantiated.

### Test Task
This is a special task, which is only started if the flag `ENABLE_TEST_MODE` is set.
It goes over the robot's devices (sensors, motors, ...), and tests them to make sure there are no
hardware defects.