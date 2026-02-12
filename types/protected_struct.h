/**
 * @file protected_struct.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <freertos/FreeRTOS.h>
#include "lib/logger/logger.h"

/**
 * @brief A RAII type used to make structs thread-safe for FreeRTOS via mutexes
 *
 * @tparam struct_type The struct to make thread-safe
 *
 * @details This class gives you the ability to make a struct thread-safe and then access it with an automatically
 *          managed FreeRTOS mutex, which automatically locks on construction and unlocks on destruction
 *          (RAII-Principle).\n\n
 *          Ways to access members of the protected struct:\n
 *          1) Single Access: Using the dereferenced pointer operator directly on the protected_struct instance
 *              @code
 *              protected_struct_instance->value = 5;
 *              @endcode
 *              This creates a temporary lock for the duration of the single expression. This is convenient for quick,
 *              atomic operations but can be inefficient for multiple accesses within a single logical block.
 *
 *          2) Full Scope Access: Using the @c get() method to obtain a locked guard object
 *              @code
 *              auto locked_data = protected_struct_instance.get();
 *              locked_data->value = 5;
 *              // ... more operations on locked_data-> ...
 *              @endcode
 *              This acquires the mutex and locks the struct for the entire lifetime of the @c locked_data object.
 *              The mutex is automatically released when @c locked_data goes out of scope (RAII). This is efficient
 *              for performing multiple operations on the protected struct within a single logical block.
 */
template <typename struct_type>
class protected_struct {
private:
    static constexpr const char* TAG = "protected_struct";

    struct_type data;
    mutable SemaphoreHandle_t mutex;

    struct locked_access_guard {
        const protected_struct* parent;

        explicit locked_access_guard(const protected_struct* parent) : parent(parent) {
            if (parent->mutex == nullptr) {
                parent->mutex = xSemaphoreCreateMutex();
            }
            xSemaphoreTake(parent->mutex, portMAX_DELAY);
        }

        ~locked_access_guard() {
            unlock();
        }

        void unlock() {
            if (parent != nullptr) {
                xSemaphoreGive(parent->mutex);
                parent = nullptr;
            }
        }

        struct_type* operator->() const {
            return (parent != nullptr) ? const_cast<struct_type*>(&parent->data) : nullptr;
        }

        operator struct_type&() const {
            return *const_cast<struct_type*>(&parent->data);
        }
    };

public:
    explicit protected_struct() : mutex() {}
    ~protected_struct() {
        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
        }
    }

    protected_struct(const protected_struct&) = delete;
    protected_struct& operator=(const protected_struct&) = delete;

    /**
     * @brief Overload of the get value from dereferenced pointer operator to make it
     *        possible to get a value using it
     *
     * @return A temporary @c locked_access_guard object to hold the mutex and return the value
     */
    locked_access_guard operator->() const {
        return locked_access_guard(this);
    }

    /**
     * @brief Locks the struct and holds the mutex for the returned guard's scope
     *
     * @return A @c locked_access_guard object to hold the mutex and provide access to the protected data
     */
    locked_access_guard lock() const {
        return locked_access_guard(this);
    }
};