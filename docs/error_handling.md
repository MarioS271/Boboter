# Error Handling

## Native ESP-IDF Errors
For functions returning this type of error code (like `gpio_config()` or other built-in ESP-IDF functions),
the library `error` has macros called `ERROR_CHECK` and `WARN_CHECK` which inspect the return value
and if necessary either print a warning to the log (`WARN_CHECK`) or abort the program entirely (`ERROR_CHECK`).

Normally, you will never cross paths with these functions, as they're only necessary in hardware abstraction layers and
component libraries.


## Custom Error Types
For custom errors, there are three types of errors:
 - Programming Errors
 - Runtime Errors
 - Runtime Exceptions

### Programming Errors
These are errors which form due to illegal programming, such as not using correct types or passing an unusable
GPIO pin, ADC channel or similar.
When such errors are encountered, the firmware will immediately abort execution and print logs to
the serial console.

### Runtime Errors
These are errors which form due to things like invalid data being recorded or sent from somewhere.
It gets handled and warned about, and the firmware continues executing,
often with a zeroed, empty or approximated value in place of the actual value.

### Runtime Exceptions
These are errors which form due to a failiure such as a critical component not initializing correctly.
When such errors happen, the firmware will immediately abort execution and print logs to the serial console.