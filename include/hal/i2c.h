/**
 * @file i2c.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#pragma once

#include <vector>
#include <soc/gpio_num.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief A namespace containing all components of the I2C hardware abstraction layer
 */
namespace HAL::I2C {
    struct bus_config_t {
        i2c_port_num_t port;
        gpio_num_t sda_pin;
        gpio_num_t scl_pin;
    };

    struct device_config_t {
        uint16_t address;
        uint32_t clock_speed;
        i2c_addr_bit_len_t address_bit_length;
    };

    /**
     * @brief The I2C hardware abstraction layer's control class
     */
    class Controller {
    private:
        static constexpr const char* TAG = "HAL::I2C";

        mutable SemaphoreHandle_t mutex;

        bus_config_t config;
        bool is_configured;

        i2c_master_bus_handle_t bus_handle = nullptr;

        std::vector<i2c_master_dev_handle_t> registered_devices;

    private:
        explicit Controller();
        ~Controller();

    public:
        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;

        /**
         * @brief Returns a reference to the static controller instance
         * @note The instance will be created on the first call of this function
         *
         * @return Reference to the controller object
         */
        static Controller& get_instance() {
            static Controller _instance;
            return _instance;
        }

        /**
         * @brief Configures the I2C controller
         *
         * @param config The config struct to use
         */
        void configure(const bus_config_t& config);

        /**
         * @brief Safely shuts down the I2C controller and removes all devices
         */
        void shutdown();

        /**
         * @brief Scans for I2C devices and outputs all found addresses to the serial log
         */
        void scan_for_devices() const;

        /**
         * @brief Adds a device to the I2C bus and returns its handle
         * @note If controller is not registered, @c nullptr will
         *       be returned, which will likely bring the firmware to crash later in the program
         *
         * @return The created device handle
         *
         * @param config The device configuration
         */
        i2c_master_dev_handle_t add_device(const device_config_t& config);

        /**
         * @brief Gets the controller's I2C bus handle
         *
         * @return The bus handle
         */
        [[nodiscard]] i2c_master_bus_handle_t get_bus_handle() const { return bus_handle; }

        /**
         * @brief Writes data to a specific device
         *
         * @param device_handle The handle of the target device
         * @param data Pointer to the buffer containing the data to send
         * @param size Number of bytes to write
         * @param timeout_ms Operation timeout in milliseconds (default: -1)
         */
        void write(i2c_master_dev_handle_t device_handle, const uint8_t* data, size_t size, int32_t timeout_ms = -1) const;

        /**
         * @brief Reads data from a specific device (For reading data, @c write_read should be preferred, this
         *        function only exists to add support for legacy devices)
         *
         * @param device_handle The handle of the target device
         * @param buffer Pointer to the buffer where the read data will be stored
         * @param size Number of bytes to read
         * @param timeout_ms Operation timeout in milliseconds (default: -1)
         */
        void read(i2c_master_dev_handle_t device_handle, uint8_t* buffer, size_t size, int32_t timeout_ms = -1) const;

        /**
         * @brief Write-read operation (Repeated Start, Recommended for reading on most devices)
         *
         * @param device_handle The handle of the target device
         * @param write_data Buffer containing the data to send (register address, ...)
         * @param write_size Number of bytes to write before starting the read
         * @param read_buffer Buffer where the recieved datawill be stored
         * @param read_size Number of bytes to read
         * @param timeout_ms Operation timeout in milliseconds (default: -1)
         */
        void write_read(i2c_master_dev_handle_t device_handle, const uint8_t* write_data, size_t write_size, uint8_t* read_buffer, size_t
                        read_size, int32_t timeout_ms = -1) const;
    };
}