/*******************************************************************************
 * protocol.h — AA55 编码
 ******************************************************************************/
#pragma once

#include <cstdint>
#include <string>

namespace protocol {

uint16_t crc16_modbus(const uint8_t *data, size_t len);
std::string build_frame_hex(uint8_t addr, uint8_t cmd,
                            const uint8_t *payload, size_t plen);
std::string pack_motion(int x, int y, int yaw, int depth);
std::string pack_servo(int idx, int pwm);
std::string pack_gripper(int dir);

}  // namespace protocol
