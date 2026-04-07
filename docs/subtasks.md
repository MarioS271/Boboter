# Subtasks

This document details the subtasks, which are lightweight routines managed by the `Subtask Scheduler Task`
(see [tasks.md](tasks.md)). These subtasks share the resources of their managing FreeRTOS task, significantly
reducing RAM usage compared to individual FreeRTOS tasks. They are designed to be non-blocking and execute quickly,
yielding control back to the `Subtask Scheduler Task` when they need to wait.
The return value of the subtasks tells the scheduler how long it should wait until re-executing that subtask.

## Overview of Subtasks
| Task               | Run Condition                                                                |
|--------------------|------------------------------------------------------------------------------|
| Display Subtask    | If the flag `ENABLE_DISPLAY` is **true** and `ENABLE_TEST_MODE` is **false** |
| Status LED Subtask | Always runs                                                                  |
| RGB LEDs Subtask   | If the flag `ENABLE_TEST_MODE` is **false**                                  |
| Buzzer Subtask     | If the flag `ENABLE_BUZZER` is **true** and `ENABLE_TEST_MODE` is **false**  |

### Display Subtask
Manages the built-in display and interaction with it via the two buttons

### Status LED Subtask
Implements various modes for the main status LED (e.g., solid on, off, slow blink, fast blink).

### RGB LEDs Subtask
Executes dynamic lighting routines and patterns on the RGB LED strips.

### Buzzer Subtask
Handles the playback of individual tones or complex sound sequences.
