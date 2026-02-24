/**
 * @file imu_registers.inc.h
 *
 * @authors MarioS271
 * @copyright AGPLv3 License
 */

static constexpr uint8_t REG_PWR_MGMT_1    = 0x6B;
static constexpr uint8_t REG_WHO_AM_I      = 0x75;
static constexpr uint8_t REG_BANK_SEL      = 0x6D;
static constexpr uint8_t REG_MEM_RW        = 0x6E;
static constexpr uint8_t REG_MEM_RW_DATA   = 0x6F;
static constexpr uint8_t REG_USER_CTRL     = 0x6A;
static constexpr uint8_t REG_FIFO_EN       = 0x23;
static constexpr uint8_t REG_FIFO_COUNT_H  = 0x72;
static constexpr uint8_t REG_FIFO_COUNT_L  = 0x73;
static constexpr uint8_t REG_FIFO_R_W      = 0x74;
static constexpr uint8_t REG_DMP_START_H   = 0x70;
static constexpr uint8_t REG_DMP_START_L   = 0x71;
static constexpr uint8_t REG_SMPLRT_DIV    = 0x19;

static constexpr uint8_t CMD_DEVICE_RESET       = 0x80;
static constexpr uint8_t CMD_SET_CLK_TO_GYRO_X  = 0x01;
static constexpr uint8_t CMD_DMP_EN             = 0x80;
static constexpr uint8_t CMD_FIFO_EN            = 0x40;
static constexpr uint8_t CMD_FIFO_RESET         = 0x04;