/*******************************************************************************
 * config.h — AA55 协议常量（不含网络参数）
 ******************************************************************************/
#pragma once

#include <cstdint>

namespace config {

inline constexpr uint8_t FRAME_HDR1 = 0xAA;
inline constexpr uint8_t FRAME_HDR2 = 0x55;

inline constexpr uint8_t ADDR_PROPELLER = 0x02;
inline constexpr uint8_t ADDR_SERVO     = 0x03;
inline constexpr uint8_t ADDR_GRIPPER   = 0x05;

inline constexpr uint8_t CMD_COMBINED_MOTION = 0x31;
inline constexpr uint8_t CMD_SET_SERVO_PWM   = 0x03;
inline constexpr uint8_t CMD_GRIPPER_SPEED   = 0x00;

}  // namespace config
