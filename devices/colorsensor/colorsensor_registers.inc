/**
 * @file colorsensor_registers.inc
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

static constexpr uint8_t COMMAND_BIT = 0x80;
static constexpr uint8_t AUTO_INC    = 0x20; // Type: 01 (Auto-increment protocol)

static constexpr uint8_t REG_ENABLE      = 0x00;
static constexpr uint8_t REG_ATIME       = 0x01;
static constexpr uint8_t REG_WTIME       = 0x03;
static constexpr uint8_t REG_AILT_L      = 0x04; // Clear interrupt low threshold
static constexpr uint8_t REG_AILT_H      = 0x05;
static constexpr uint8_t REG_AIHT_L      = 0x06; // Clear interrupt high threshold
static constexpr uint8_t REG_AIHT_H      = 0x07;
static constexpr uint8_t REG_PERS        = 0x0C; // Interrupt persistence filter
static constexpr uint8_t REG_CONFIG      = 0x0D;
static constexpr uint8_t REG_CONTROL     = 0x0F; // Gain control
static constexpr uint8_t REG_ID          = 0x12; // Should read 0x44 (TCS34725) or 0x4D (TCS34721)
static constexpr uint8_t REG_STATUS      = 0x13;

static constexpr uint8_t REG_CLEAR_LOW   = 0x14; // Clear channel data
static constexpr uint8_t REG_CLEAR_HIGH  = 0x15;
static constexpr uint8_t REG_RED_LOW     = 0x16; // Red channel data
static constexpr uint8_t REG_RED_HIGH    = 0x17;
static constexpr uint8_t REG_GREEN_LOW   = 0x18; // Green channel data
static constexpr uint8_t REG_GREEN_HIGH  = 0x19;
static constexpr uint8_t REG_BLUE_LOW    = 0x1A; // Blue channel data
static constexpr uint8_t REG_BLUE_HIGH   = 0x1B;

static constexpr uint8_t CMD_ENABLE_AIEN = 0x10; // RGBC Interrupt Enable
static constexpr uint8_t CMD_ENABLE_WEN  = 0x08; // Wait Enable
static constexpr uint8_t CMD_ENABLE_AEN  = 0x02; // RGBC Enable
static constexpr uint8_t CMD_ENABLE_PON  = 0x01; // Power ON

static constexpr uint8_t CMD_GAIN_1X     = 0x00;
static constexpr uint8_t CMD_GAIN_4X     = 0x01;
static constexpr uint8_t CMD_GAIN_16X    = 0x02;
static constexpr uint8_t CMD_GAIN_60X    = 0x03;