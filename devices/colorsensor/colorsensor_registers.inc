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
static constexpr uint8_t REG_CONTROL     = 0x0F; // Gain control
static constexpr uint8_t REG_ID          = 0x12; // ID register (should be TCS34725_DEVICE_ID)
static constexpr uint8_t REG_CLEAR_LOW   = 0x14;

static constexpr uint8_t CMD_ENABLE_AEN  = 0x02; // RGBC Enable
static constexpr uint8_t CMD_ENABLE_PON  = 0x01; // Power ON

static constexpr uint8_t GAIN_1X     = 0x00;
static constexpr uint8_t GAIN_4X     = 0x01;
static constexpr uint8_t GAIN_16X    = 0x02;
static constexpr uint8_t GAIN_60X    = 0x03;