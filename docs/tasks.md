# Tasks
The firmware is segmented into a few FreeRTOS tasks to seperate and categorize the individual workloads.

Here is an overview of all tasks:
| Task Name         | Priority | Stack Depth | When is it created | Where is it created | On what condition is it created (- for none) |
|-------------------|----------|-------------|--------------------|---------------------|----------------------------------------------|
| Secure Task       | 25 | 1024 | Once | `src/main.cpp` | - |
| System Task       | 20 | 2048 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to false |
| PID Task          | 15 | 4096 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to false |
| IO Task           | 10 | 2048 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to false |
| Bluepad Task      |  8 | 4096 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to false |
| LEDs Task         |  5 | 2048 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to false |
| Buzzer Task       |  1 | 1024 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to false |
| Ramp Task         | 14 | 2048 | Once for each instance of the `Motor` class | `libs/motor/motor.cpp` | - |
| Sensor Test Task  | 22 | 8192 | Once | `src/main.cpp` | If the flag `ENABLE_SENSOR_TEST_MODE` is set to true |


### Secure Task
The secure task is for making sure that the system does not wreck itsself,
by for example making sure to prevent battery over-discharge.

### System Task
This task coordinates everything. For instance, it acts as the interface between
the Bluepad Task and the PID Task: When controller inputs are received in the
Bluepad Task and written to a flex, the Control Task reads that flex and signals
the PID Task to take action. It also controls miscellaneous LEDs such as the status LED.

### PID Task
This task performs continuous PID calculations to minimize errors. It ensures the robot
maintains a stable speed even under load or decreasing battery voltage. This task
directly controls the PWM output to the motors.

### IO Task
Manages user interaction via the SSD1306 OLED display and two buttons.
Additionally, this task handles outputs such as the buzzer or the status LED.

### Bluepad Task
Receives and processes Bluetooth Gamepad Inputs.

### LEDs Task
Manages the NeoPixel RGB LEDs and executes lighting routines.

### Buzzer Task
This Task is responsible for sending the buzzer signals and playing sounds or sound
sequences.

### Ramp Task
Responsible for smooth motor acceleration and deceleration. It gradually adjusts the
target speed to prevent excessive acceleration and robot tipping.
One ramp task is created for each motor that gets instantiated.

### Sensor Test Task
This is a special task, which is only started if the flag 'ENABLE_SENSOR_TEST_MODE' is set.
It goes over the robot's sensors and capabilities, and tests them to make sure there are no
hardware defects.