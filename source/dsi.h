#pragma once

#include <cstdint>

// copied from https://github.com/CTCaer/hekate/blob/6e954f5cdff2e73d68ff2c9b463197c99544b387/bdk/display/di.h

// #define BIT(n) (1U << (n))

#define DSI_BASE 0x54300000U
#define DSI_SIZE 0x1000U

#define DSI_RD_DATA 0x9
#define DSI_WR_DATA 0xA

#define DSI_TRIGGER 0x13
#define DSI_TRIGGER_VIDEO BIT(0)
#define DSI_TRIGGER_HOST BIT(1)

#define DSI_HOST_CONTROL 0xF
#define DSI_HOST_CONTROL_ECC BIT(0)
#define DSI_HOST_CONTROL_CS BIT(1)
#define DSI_HOST_CONTROL_PKT_BTA BIT(2)
#define DSI_HOST_CONTROL_IMM_BTA BIT(3)
#define DSI_HOST_CONTROL_FIFO_SEL BIT(4)
#define DSI_HOST_CONTROL_HS BIT(5)
#define DSI_HOST_CONTROL_RAW BIT(6)
#define DSI_HOST_CONTROL_TX_TRIG_MASK (3 << 12)
#define DSI_HOST_CONTROL_TX_TRIG_SOL (0 << 12)
#define DSI_HOST_CONTROL_TX_TRIG_FIFO (1 << 12)
#define DSI_HOST_CONTROL_TX_TRIG_HOST (2 << 12)
#define DSI_HOST_CONTROL_CRC_RESET BIT(20)
#define DSI_HOST_CONTROL_FIFO_RESET BIT(21)

#define MIPI_DSI_DCS_SHORT_WRITE_PARAM 0x15

#define MIPI_DCS_SET_ADDRESS_MODE 0x36 // Display Access Control. 1 byte. 0: GS, 1: SS, 3: BGR.

#define DSI_VIDEO_MODE_CONTROL 0x4E
#define DSI_CMD_PKT_VID_ENABLE 1
#define DSI_DSI_LINE_TYPE(x) ((x) << 1)

#define DCS_ADDRESS_MODE_V_FLIP BIT(0)
#define DCS_ADDRESS_MODE_H_FLIP BIT(1)
#define DCS_ADDRESS_MODE_LATCH_RL BIT(2) // Latch Data Order.
#define DCS_ADDRESS_MODE_BGR_COLOR BIT(3)
#define DCS_ADDRESS_MODE_LINE_ORDER BIT(4) // Line Refresh Order.
#define DCS_ADDRESS_MODE_SWAP_XY BIT(5)    // Page/Column Addressing Reverse Order.
#define DCS_ADDRESS_MODE_MIRROR_X BIT(6)   // Column Address Order.
#define DCS_ADDRESS_MODE_MIRROR_Y BIT(7)   // Page Address Order.
#define DCS_ADDRESS_MODE_ROTATION_MASK (0xF << 4)
#define DCS_ADDRESS_MODE_ROTATION_90 (DCS_ADDRESS_MODE_SWAP_XY | DCS_ADDRESS_MODE_LINE_ORDER)
#define DCS_ADDRESS_MODE_ROTATION_180 (DCS_ADDRESS_MODE_MIRROR_X | DCS_ADDRESS_MODE_LINE_ORDER)
#define DCS_ADDRESS_MODE_ROTATION_270 (DCS_ADDRESS_MODE_SWAP_XY)