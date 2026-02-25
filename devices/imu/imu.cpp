/**
 * @file imu.cpp
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

#include "imu.h"

#include <esp_lcd_io_i2c.h>

#include "lib/logger/logger.h"
#include "include/robot.h"

namespace Device {
    Imu::Imu(Robot& robot) :
        robot(robot),
        device_handle(nullptr),
        quaternion_values{},
        gyro_values{}
    {
        LOGD("Constructor called");
    }

    Imu::~Imu() {
        LOGD("Destructor called");
    }

    void Imu::initialize() {
        if constexpr (!Flags::ENABLE_IMU) {
            LOGI("Flags::ENABLE_IMU is set to false, skipping IMU initialization");
            return;
        }

        device_handle = robot.i2c.add_device(
            HAL::I2C::device_config_t{
                .address = I2C_ADDRESS,
                .clock_speed = I2C_CLOCK_SPEED,
                .address_bit_length = I2C_ADDR_BIT_LEN_7
            }
        );

        write_register(REG_PWR_MGMT_1, CMD_DEVICE_RESET);
        delay(120);
        write_register(REG_PWR_MGMT_1, CMD_SET_CLK_TO_GYRO_X);

        const uint8_t device_id = read_register(REG_WHO_AM_I);
        if (device_id != MPU6050_DEVICE_ID) {
            LOGE("MPU6050 not found! Expected 0x%02x, got 0x%02x", MPU6050_DEVICE_ID, device_id);
        }
        LOGI("MPU6050 with id 0x%02x found", device_id);

        LOGI("Writing DMP firmware to IMU ...");

        uint16_t written_bytes{};
        for (uint16_t i = 0; i < DMP_FIRMWARE_SIZE; i += DMP_UPLOAD_CHUNK_SIZE) {
            const auto bank_id = static_cast<uint8_t>(i >> 8);
            const auto bank_address = static_cast<uint8_t>(i & 0xFF);

            const uint16_t remaining = DMP_FIRMWARE_SIZE - i;
            const auto current_chunk = static_cast<uint8_t>(
                std::min(
                    remaining,
                    static_cast<uint16_t>(DMP_UPLOAD_CHUNK_SIZE)
                )
            );

            write_register(REG_BANK_SEL, bank_id);
            write_register(REG_MEM_RW, bank_address);

            uint8_t write_buffer[DMP_UPLOAD_CHUNK_SIZE + 1];
            write_buffer[0] = REG_MEM_RW_DATA;
            memcpy(&write_buffer[1], &DMP_FIRMWARE[i], current_chunk);

            robot.i2c.write(device_handle, write_buffer, current_chunk + 1, 1000);
            delay(1);

            written_bytes += current_chunk;
        }

        LOGI("Finished uploading DMP firmware; %hu bytes written", written_bytes);
        LOGI("Initialized Device::Imu");
    }

    void Imu::read_fifo_buffer() {
        if constexpr (!Flags::ENABLE_IMU) {
            return;
        }

        uint8_t count_buffer[2];

        robot.i2c.write_read(device_handle, &REG_FIFO_COUNT_H, 1, count_buffer, 2, 100);
        uint16_t count = (count_buffer[0] << 8) | count_buffer[1];

        if (count < FIFO_BUFFER_SIZE) {
            LOGW("FIFO buffer is too small (<42 bytes), cancelling read");
            return;
        }

        if (count > 512) {
            write_register(REG_USER_CTRL, CMD_DMP_EN | CMD_FIFO_EN | CMD_FIFO_RESET);
            return;
        }

        uint8_t packet[FIFO_BUFFER_SIZE];
        robot.i2c.write_read(device_handle, &REG_FIFO_R_W, 1, packet, FIFO_BUFFER_SIZE, 100);

        auto parse_q30 = [&](const uint8_t offset) {
            const int32_t raw = (static_cast<int32_t>(packet[offset]) << 24) |
                                (static_cast<int32_t>(packet[offset + 1]) << 16) |
                                (static_cast<int32_t>(packet[offset + 2]) << 8) |
                                packet[offset + 3];
            return static_cast<float>(raw) / (1U << 30); // Division durch 2^30
        };

        quaternion_values.x = parse_q30(4);
        quaternion_values.y = parse_q30(8);
        quaternion_values.z = parse_q30(12);
        quaternion_values.w = parse_q30(0);

        auto parse_i16 = [&](const uint8_t offset) {
            return static_cast<int16_t>((packet[offset] << 8) | packet[offset + 1]);
        };

        gyro_values.x = parse_i16(24);
        gyro_values.y = parse_i16(26);
        gyro_values.z = parse_i16(28);

        LOGV("Read quaternion of x=%ld y=%ld z=%ld w=%ld and a gyro vector3 of x=%.2f y=%.2f z=%.2f",
            quaternion_values.x, quaternion_values.y, quaternion_values.z, quaternion_values.w,
            gyro_values.x, gyro_values.y, gyro_values.z);
    }
}
