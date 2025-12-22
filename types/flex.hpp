/**
 * @file flex.hpp
 *
 * @authors MarioS271
 * @copyright MIT License
*/

#pragma once

#include <string>
#include <unordered_map>
#include <freertos/FreeRTOS.h>

namespace Boboter::Types {
    /**
     * @class Flex
     * @brief Type to dynamically store thread-safe values identified by string keys with internal mutex handling
    */
    class flex {
    private:
        static constexpr const char* TAG = "Types::Flex";

        std::unordered_map<std::string, void*> resources;
        std::unordered_map<std::string, size_t> sizes;
        std::unordered_map<std::string, void(*)(void*)> deleters;
        mutable SemaphoreHandle_t mutex;

    public:
        explicit flex() {
            mutex = xSemaphoreCreateMutex();
        }
        flex(const flex&) = delete;
        flex& operator=(const flex&) = delete;
        
        ~flex() {
            if (!mutex) return;
            
            xSemaphoreTake(mutex, portMAX_DELAY);
            
            for (auto& pair : resources)
                deleters[pair.first](pair.second);
            
            resources.clear();
            sizes.clear();
            
            xSemaphoreGive(mutex);
            vSemaphoreDelete(mutex);
        }
        
        /**
         * @brief Sets a value in the flex struct
         *
         * @return (void)  
         *
         * @param key The key of the value to set
         * @param value The value to set
         * 
         * @tparam T The type of the value to store
        */
        template <typename T>
        void set(const std::string& key, const T& value) {
            xSemaphoreTake(mutex, portMAX_DELAY);

            auto it = resources.find(key);
            if (it != resources.end()) {
                deleters[key](it->second);
            }

            resources[key] = new T(value);
            deleters[key] = [](void* ptr) {
                delete static_cast<T*>(ptr);
            };

            xSemaphoreGive(mutex);
        }

        /**
         * @brief Fetches a value from the flex struct
         *
         * @return (T) The stored value if found and type matches, otherwise `defaultValue`
         *
         * @param key The stringkey of the element to get
         * @param defaultValue The default value to return if nothing is found
         * 
         * @tparam T The type of the value to fetch
        */
        template <typename T>
        T get(const std::string& key, const T& defaultValue = T()) const {
            xSemaphoreTake(mutex, portMAX_DELAY);

            auto it = resources.find(key);
            T result = defaultValue;

            if (it != resources.end() && sizes.find(key)->second == sizeof(T))
                result = *(T*) (it->second);

            xSemaphoreGive(mutex);
            return result;
        }

        /**
         * @brief Check if the flex struct contains a certain value
         *
         * @return (bool) If the value was found
         *
         * @param key The key to search for
        */
        bool contains(const std::string& key) const {
            xSemaphoreTake(mutex, portMAX_DELAY);
            bool found = (resources.find(key) != resources.end());
            xSemaphoreGive(mutex);
            return found;
        }
    };
}