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
 * @brief A type used to make structs thread-safe for FreeRTOS via mutexes
 *
 * @tparam struct_type The struct to make thread-safe
 *
 * Creating a protected struct called @c y from a normal struct called @c x:
 * @code protected_struct<x> y;@endcode
 *
 * Accessing a member:
 * @code y->value = 5;@endcode
 *
 * Passing the whole struct:
 * @code func_that_needs_struct(*y);@endcode
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