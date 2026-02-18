/**
 * @file protected_struct.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <freertos/FreeRTOS.h>
#include "helpers/halt_execution.h"
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
 *          
 *          1) Single Access: Using the dereferenced pointer operator directly on the protected_struct instance
 *              @code
 *              protected_struct_instance->value = 5;
 *              @endcode
 *              This creates a temporary lock for the duration of the single expression. This is convenient for quick,
 *              atomic operations but can be inefficient for multiple accesses within a single logical block.
 *
 *          2) Full Scope Access: Using the @c lock() method to obtain a mutex lock object
 *              @code
 *              auto locked_data = protected_struct_instance.lock();
 *              locked_data->value = 5;
 *              // ... more operations on locked_data-> ...
 *              @endcode
 *              This acquires the mutex and locks the struct for the entire lifetime of the @c locked_data object.
 *              The mutex is automatically released when @c locked_data goes out of scope. This is efficient
 *              for performing multiple operations on the protected struct within a single logical block.
 *              You can also release the mutex early by calling @c locked_data.unlock()
 */
template <typename struct_type>
class protected_struct {
private:
    static constexpr const char* TAG = "protected_struct";

    struct_type data;
    mutable SemaphoreHandle_t mutex;

    class mutex_lock {
    public:
        protected_struct* parent;

        explicit mutex_lock(const protected_struct* parent) :
            parent(const_cast<protected_struct*>(parent))
        {
            if (xSemaphoreGetMutexHolder(parent->mutex) == xTaskGetCurrentTaskHandle()) {
                LOGE("Mutex deadlock detected, aborting");
                halt_execution();
            }

            xSemaphoreTake(parent->mutex, portMAX_DELAY);
        }

        ~mutex_lock() {
            unlock();
        }

        /**
         * @brief Unlocks the mutex ahead of scope-end
         */
        void unlock() {
            if (parent != nullptr) {
                xSemaphoreGive(parent->mutex);
                parent = nullptr;
            }
        }

        struct_type* operator->() {
            return parent != nullptr ? &parent->data : nullptr;
        }

        operator struct_type&() {
            return parent->data;
        }
    };

public:
    explicit protected_struct() : mutex(xSemaphoreCreateMutex()) {
        if (mutex == nullptr) {
            LOGE("Mutex creation failed");
            halt_execution();
        }
    }
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
     * @return A temporary @c mutex_lock object to hold the mutex and return the value
     */
    mutex_lock operator->() const {
        return mutex_lock(this);
    }

    /**
     * @brief Locks the struct and holds the mutex for the returned object's scope
     *
     * @return A @c mutex_lock object to hold the mutex and provide access to the protected data
     */
    mutex_lock lock() const {
        return mutex_lock(this);
    }
};