# Status LED modes

| Blink Mode   | Meaning         | Explanation                                                                               |
|--------------|-----------------|-------------------------------------------------------------------------------------------|
| `OFF`        | Not operational | The robot is either turned off, in deep sleep or has encountered an unrecoverable error   |
| `ON`         | All good        | Everything is working fine as it is supposed to be                                        |
| `BLINK_SLOW` | Connecting      | The robot is connecting to either the gamepad or the app                                  |
| `BLINK_FAST` | Battery Low     | The robot's battery is low, it will enter deep sleep in a few seconds to conserve battery |