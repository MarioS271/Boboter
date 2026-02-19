# Cooperative Subtasks

This document details the cooperative subtasks, which are lightweight routines managed by the `Subtask Handler Task`
(see [tasks.md](tasks.md)). These subtasks share the resources of their managing FreeRTOS task, significantly
reducing RAM usage compared to individual FreeRTOS tasks. They are designed to be non-blocking and execute quickly,
yielding control back to the `Subtask Handler Task` when they need to wait.

## Overview of Cooperative Subtasks
| Task               | Run Condition                                             |
|--------------------|-----------------------------------------------------------|
| Display Subtask    | Only runs if the flag `ENABLE_DISPLAY` is set to **true** |
| Status LED Subtask | Always runs                                               |
| RGB LEDs Subtask   | Always runs                                               |
| Buzzer Subtask     | Always runs                                               |

### Display Subtask
Manages the built-in display and interaction with it via the two buttons

### Status LED Subtask
Implements various modes for the main status LED (e.g., solid on, off, slow blink, fast blink).

### RGB LEDs Subtask
Executes dynamic lighting routines and patterns on the RGB LED strips.

### Buzzer Subtask
Handles the playback of individual tones or complex sound sequences.
