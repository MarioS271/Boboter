/**
 * @file protected_struct.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once
#include "freertos/FreeRTOS.h"

/**
 * @brief A type used to make structs thread-safe for FreeRTOS via mutexes
 *
 * @tparam struct_type The struct to make thread-safe
 *
 * @code
 * protected_struct<x> y;
 * @endcode
 * Creating a protected struct called @c y from a normal struct called @c x
 *
 * @code
 * y->value = 5;
 * @endcode
 * Accessing a member
 *
 * @code
 * func_that_needs_struct(*y);
 * @endcode
 * Passing the whole struct
 */
template <typename struct_type>
class protected_struct {
private:
    struct_type data;
    SemaphoreHandle_t mutex;

    struct proxy {
        protected_struct& parent;

        explicit proxy(protected_struct& parent) : parent(parent) { xSemaphoreTake(parent.mutex, portMAX_DELAY); }
        ~proxy() { xSemaphoreGive(parent.mutex); }

        /**
         * @brief Operator Overload to retrieve automatically dereferenced pointer to data
         */
        struct_type* operator->() { return &parent.data; }

        /**
         * @brief Operator Overload to enable retrieving a reference to the data to be able to
         *        pass it somewhere
         */
        operator struct_type&() { return parent.data; }
    };

public:
    explicit protected_struct() : mutex(xSemaphoreCreateMutex()) {}
    ~protected_struct() {
        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
        }
    }

    protected_struct(const protected_struct&) = delete;
    protected_struct& operator=(const protected_struct&) = delete;

    /**
     * @brief Overload of the @c -> (get value from dereferenced pointer) operator to make it
     *        possible to get a value using it
     *
     * @return A temporary proxy object to hold the mutex and return the value
     */
    proxy operator->() {
        return proxy(*this);
    }

    /**
     * @brief Overload of the @c * (dereference) operator to make it possible to pass the
     *        whole struct to something
     *
     * @return A temporary proxy object to hold the mutex and return the value
     */
    proxy operator*() {
        return proxy(*this);
    }
};