/**
 * @file flex.h
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#pragma once

#include <string>
#include <unordered_map>
#include <freertos/FreeRTOS.h>

/**
 * @brief A type used to store thread-safe string-keyed values in an unordered map via mutexes
 */
class flex {
private:
    struct resource_entry {
        void* pointer;
        size_t size;
        void (*deleter)(void*);
    };

    static constexpr const char* TAG = "Type:Flex";

    std::unordered_map<std::string, resource_entry> resources;
    mutable SemaphoreHandle_t mutex;

public:
    explicit flex() {
        mutex = xSemaphoreCreateMutex();
    }
    flex(const flex&) = delete;
    flex& operator=(const flex&) = delete;
    
    ~flex() {
        if (!mutex)
            return;
        
        xSemaphoreTake(mutex, portMAX_DELAY);
        
        for (const auto& pair : resources) {
            pair.second.deleter(pair.second.pointer);
        }
        
        resources.clear();
        
        xSemaphoreGive(mutex);
        vSemaphoreDelete(mutex);
    }
    
    /**
     * @brief Sets a value in the flex struct
     *
     * @param key The key of the value to set
     * @param value The value to set
     * 
     * @tparam T The type of the value to store
     */
    template <typename T>
    void set(const std::string& key, const T& value) {
        if (mutex == nullptr)
            abort();

        xSemaphoreTake(mutex, portMAX_DELAY);

        const auto it = resources.find(key);
        if (it != resources.end()) {
            it->second.deleter(it->second.pointer);
            it->second.pointer = new T(value);
            it->second.size = sizeof(T);
            it->second.deleter = [](void* ptr) { delete static_cast<T*>(ptr); };
        } else {
            resource_entry entry;
            entry.pointer = new T(value);
            entry.size = sizeof(T);
            entry.deleter = [](void* ptr) { delete static_cast<T*>(ptr); };
            resources[key] = entry;
        }

        xSemaphoreGive(mutex);
    }

    /**
     * @brief Fetches a value from the flex struct
     *
     * @return If found and type is correct, the stored value, otherwise @c default_value
     *
     * @param key The string key of the element to get
     * @param default_value The default value to return if nothing is found
     * 
     * @tparam T The type of the value to fetch
     */
    template <typename T>
    T get(const std::string& key, const T& default_value = T()) const {
        if (mutex == nullptr)
            return default_value;
        
        xSemaphoreTake(mutex, portMAX_DELAY);

        const auto it = resources.find(key);
        T result = default_value;

        if (it != resources.end() && it->second.size == sizeof(T))
            result = *static_cast<T*>(it->second.pointer);

        xSemaphoreGive(mutex);
        return result;
    }

    /**
     * @brief Erases one element from the flex
     *
     * @param key The element's string key
     */
    void erase(const std::string& key) {
        if (mutex == nullptr)
            return;

        xSemaphoreTake(mutex, portMAX_DELAY);
        
        auto it = resources.find(key);
        if (it != resources.end()) {
            it->second.deleter(it->second.pointer);
            resources.erase(it);
        }
        
        xSemaphoreGive(mutex);
    }

    /**
     * @brief Check if the flex struct contains a certain value
     *
     * @return Whether the value was found (true) or not (false)
     *
     * @param key The key to search for
     */
    bool contains(const std::string& key) const {
        if (mutex == nullptr)
            return false;

        xSemaphoreTake(mutex, portMAX_DELAY);
        bool found = (resources.find(key) != resources.end());
        xSemaphoreGive(mutex);

        return found;
    }
};