# Error Handling

## Native ESP-IDF errors
For functions returning this type of error code (like `gpio_config()` or other built-in ESP-IDF functions),
the library `error` has inline functions called `ERROR_CHECK` and `WARN_CHECK` which inspect the return value
and if necessary either print a warning to the log (`WARN_CHECK`) or abort the program entirely (`ERROR_CHECK`).

You will rarely if ever encounter such functions though, as everything is wrapped in HALs or similar,
so if you actually need to use these functions, you should rethink the code as something probably isn't right.


## Custom errors (implemented by this firmware)
For custom errors, there are two types of errors:
 - Runtime Errors
 - Program Errors

### Runtime Errors
These are errors which form due to invalid data being recorded or sent from somewhere.

It gets handled and warned about, and the firmware continues executing,
often with a zeroed or empty value in place of the actual value.

### Program Errors
These are errors which form due to illegal programming, such as not using correct types or passing a unusable GPIO pin, ADC channel or similar.

When such errors are encountered, the firmware will immediately abort execution and print logs to the serial console.