/**
 * @file smart_mutex.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <freertos/FreeRTOS.h>

/**
 * @brief A RAII wrapper for mutexes which locks them at the beginning of its lifetime and releases them
 *        at the end of its lifetime (normally the scope in which the wrapper was created)
 *
 * @details To create a smart mutex, just do: @code smart_mutex lock(mutex);@endcode
 *          Done! Now, the mutex will stay locked until the function finishes (until this object goes out
 *          of scope and its destructor is called) and then it will automatically unlock the mutex for you.
 *          Be careful that you don't lock the mutex and then call a function which again locks the mutex,
 *          which will cause the firmware to hang!!
 */
class smart_mutex {
private:
    SemaphoreHandle_t mutex;

public:
    explicit smart_mutex(SemaphoreHandle_t mutex) : mutex(mutex) {
        xSemaphoreTake(mutex, portMAX_DELAY);
    }

    ~smart_mutex() {
        xSemaphoreGive(mutex);
    }

    smart_mutex(const smart_mutex&) = delete;
    smart_mutex& operator=(const smart_mutex&) = delete;
};
