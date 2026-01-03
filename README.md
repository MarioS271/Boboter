# Boboter

An ESP-IDF based firmware for the robot built in the HTL St. Pölten's EL department.

When using CLion, the project comes with configurations for building, uploading and generating doxygen.
If you use another IDE, all these things can also be done over the terminal.

### Documentation
To view generated doxygen documentation, open the file `/docs/doxygen/index.html`.
Other documentation, such as for error handling or tasks is located in `/docs/`.

### Commands
| Command             | For what it's used                                       |
|---------------------|----------------------------------------------------------|
| `pio run`           | Build the ESP32 firmware binary                          |
| `pio run -t upload` | Build and Upload the ESP32 firmware binary               |
| `doxygen Doxyfile`  | Build the doxygen documentation using the given doxyfile |

<br><hr><br>
Licensed under the MIT License