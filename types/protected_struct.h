/**
 * @file protected_struct.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <freertos/FreeRTOS.h>
#include "lib/logger/logger.h"

/**
 * @brief A RAII type used to make structs thread-safe for FreeRTOS via mutexes
 * @note Always use the @c -> operator when accessing members!
 *
 * @tparam struct_type The struct to make thread-safe
 *
 * @details This class gives you the ability to make a struct thread-safe and then access it with an automatically
 *          managed FreeRTOS mutex, which automatically locks on construction and unlocks on destruction
 *          (RAII-Principle)\n\n
 *          Ways to access members of the protected struct:\n
 *          1) Single Access: Get from dereferenced pointer operator
 *              @code
 *              data->value = 5;
 *              @endcode
 *              Accesses and locks the struct for only one code line, very inefficient if you have lots of operations
 *              on the struct\n\n
 *          2) Full Scope Access: Dereference Operator
 *              @code
 *              const auto ldata = *data;
 *              ldata->value = 5;
 *              @endcode
 *              Locks the struct for the full lifetime of the current scope, lets you interact with the locked
 *              struct via the newly variable (in this case: @c ldata)\n\n
 *          3) Passing protected struct to function: Dereference Operator
 *              @code
 *              void process(my_struct& s) {
 *                  s.value = 5;
 *              }
 *              process(*data);
 *              @endcode
 *              Locks the struct, passes the locked struct to the function, executes the function and unlocks the
 *              struct again.\n
 *              IMPORTANT: Here, we use @c s.value instead of @c s->value in the function, as we're getting a struct reference
 *                         instead of a struct pointer.
 *
 * @attention If you try to do something like this:
 *            @code
 *            const auto ldata = *data;
 *
*             void process(my_struct& s) {
 *                s.value = 5;
 *            }
 *            process(*data);
 *            @endcode
 *            The firmware will get caught in a race condition which is known as a deadlock, meaning it will
 *            try double-locking the mutex, which leads to it permanently hanging as soon as @c process(*data); is
 *            called. This happens because when you first create the ldata object, data gets locked. If you
 *            now also pass data to the @c process() function using the dereference operator (@c *data), the mutex will
 *            try to lock for a second time and wait forever until it's released for the first time, which can't happen,
 *            because the code flow to the end of scope is blocked by the mutex waiting for its own release.\n
 *            To fix this issue, instead of passing @c *data, just pass the already locked @c ldata object.
 */
template <typename struct_type>
class protected_struct {
private:
    static constexpr const char* TAG = "protected_struct";

    struct_type data;
    mutable SemaphoreHandle_t mutex;

    struct proxy {
        const protected_struct& parent;

        explicit proxy(const protected_struct& parent) : parent(parent) {
            if (parent.mutex == nullptr) {
                parent.mutex = xSemaphoreCreateMutex();
            }
            if (parent.mutex != nullptr) {
                xSemaphoreTake(parent.mutex, portMAX_DELAY);
            }
        }
        ~proxy() { xSemaphoreGive(parent.mutex); }

        /**
         * @brief Operator Overload to retrieve automatically dereferenced pointer to data
         */
        struct_type* operator->() const {
            return const_cast<struct_type*>(&parent.data);
        }

        /**
         * @brief Operator Overload to enable retrieving a reference to the data to be able to
         *        pass it somewhere
         */
        operator struct_type&() const {
            return *const_cast<struct_type*>(&parent.data);
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
     * @return A temporary proxy object to hold the mutex and return the value
     */
    proxy operator->() const {
        return proxy(*this);
    }

    /**
     * @brief Overload of the dereference operator to make it possible to pass the
     *        whole struct to something
     *
     * @return A temporary proxy object to hold the mutex and return the value
     */
    proxy operator*() const {
        return proxy(*this);
    }
};