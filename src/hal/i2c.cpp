/**
 * @file i2c.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/hal/i2c.h"

#include "types/smart_mutex.h"
#include "lib/logger/logger.h"
#include "lib/error/error.h"

namespace HAL::I2C {
    Controller::Controller() :
        mutex(xSemaphoreCreateMutex()),
        config(),
        is_configured(false)
    {
        LOGI("Constructor of HAL::I2C::Controller called");
    }

    Controller::~Controller() {
        LOGI("Destructor of HAL::I2C::Controller called");

        shutdown();

        if (mutex != nullptr) {
            vSemaphoreDelete(mutex);
        }
    }

    void Controller::configure(const bus_config_t& config) {
        if (is_configured) {
            LOGW("Unable to configure I2C controller, controller is already configured");
            return;
        }

        smart_mutex lock(mutex);

        const i2c_master_bus_config_t bus_config = {
            .i2c_port = config.port,
            .sda_io_num = config.sda_pin,
            .scl_io_num = config.scl_pin,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .intr_priority = 0,
            .trans_queue_depth = 0,
            .flags = { .enable_internal_pullup = 1 }
        };
        ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

        this->config = config;
        is_configured = true;

        LOGI("Initialized HAL::I2C::Controller, configured bus %d", static_cast<uint8_t>(config.port));
    }

    void Controller::shutdown() {
        if (!is_configured) {
            LOGW("Unable to shut down I2C controller, controller is not configured");
            return;
        }

        smart_mutex lock(mutex);

        for (const auto device_handle : registered_devices) {
            WARN_CHECK(i2c_master_bus_rm_device(device_handle));
        }

        registered_devices.clear();

        if (bus_handle != nullptr) {
            WARN_CHECK(i2c_del_master_bus(bus_handle));
            bus_handle = nullptr;
        }

        config = {};
        is_configured = false;

        LOGI("Shut down HAL::I2C::Controller");
    }

    void Controller::scan_for_devices() const {
        if (!is_configured) {
            LOGW("Unable to scan for devices, controller is not configured");
            return;
        }

        smart_mutex lock(mutex);

        LOGI("Addresses in I2C bus %d:", static_cast<uint8_t>(config.port));
        uint8_t count = 0;

        for (uint16_t i = 0x03; i < 0x78; ++i) {
            if (i2c_master_probe(bus_handle, i, 100) != ESP_OK)
                continue;

            LOGI("  -> 0x%02x", i);
            ++count;
        }

        LOGI("I2C scan finished, found %d devices", count);
    }

    i2c_master_dev_handle_t Controller::add_device(const device_config_t& config) {
        if (!is_configured) {
            LOGW("Unable to add device, controller is not configured (returning nullptr, firmware will most likely fail at some point because of this)");
            return nullptr;
        }

        smart_mutex lock(mutex);

        const i2c_device_config_t device_config = {
            .dev_addr_length = config.address_bit_length,
            .device_address = config.address,
            .scl_speed_hz = config.clock_speed
        };

        i2c_master_dev_handle_t device_handle;
        ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &device_config, &device_handle));

        registered_devices.push_back(device_handle);

        return device_handle;
    }

    void Controller::write(
        const i2c_master_dev_handle_t device_handle,
        const uint8_t* data,
        const size_t size,
        const int32_t timeout_ms) const
    {
        ERROR_CHECK(i2c_master_transmit(device_handle, data, size, timeout_ms));
    }

    void Controller::read(
        const i2c_master_dev_handle_t device_handle,
        uint8_t* buffer,
        const size_t size,
        const int32_t timeout_ms) const
    {
        ERROR_CHECK(i2c_master_receive(device_handle, buffer, size, timeout_ms));
    }

    void Controller::write_read(
        const i2c_master_dev_handle_t device_handle,
        const uint8_t* write_data,
        const size_t write_size,
        uint8_t* read_buffer,
        const size_t read_size,
        const int32_t timeout_ms) const
    {
        ERROR_CHECK(i2c_master_transmit_receive(device_handle, write_data, write_size, read_buffer, read_size, timeout_ms));
    }
}