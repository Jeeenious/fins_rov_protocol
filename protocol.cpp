/*******************************************************************************
 * protocol.cpp — AA55 编码实现
 ******************************************************************************/

#include "protocol.h"
#include "config.h"
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

namespace protocol {

uint16_t crc16_modbus(const uint8_t *data, size_t len) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < len; ++i) {
    crc ^= data[i];
    for (int b = 0; b < 8; ++b)
      crc = (crc & 0x0001) ? ((crc >> 1) ^ 0xA001) : (crc >> 1);
  }
  return crc;
}

std::string build_frame_hex(uint8_t addr, uint8_t cmd,
                            const uint8_t *payload, size_t plen) {
  if (plen > 255) return "";
  uint8_t body[259];
  body[0]=addr; body[1]=cmd; body[2]=static_cast<uint8_t>(plen);
  if (payload && plen) std::memcpy(body+3, payload, plen);
  size_t blen = 3+plen;
  uint16_t crc = crc16_modbus(body, blen);

  std::vector<uint8_t> f;
  f.push_back(config::FRAME_HDR1);
  f.push_back(config::FRAME_HDR2);
  f.insert(f.end(), body, body+blen);
  f.push_back(static_cast<uint8_t>(crc & 0xFF));
  f.push_back(static_cast<uint8_t>((crc>>8) & 0xFF));

  std::ostringstream oss; oss << std::hex << std::setfill('0');
  for (uint8_t b : f) oss << std::setw(2) << static_cast<int>(b);
  return oss.str();
}

static void w16(uint8_t *b, int16_t v) { b[0]=v&0xFF; b[1]=(v>>8)&0xFF; }

std::string pack_motion(int x, int y, int yaw, int depth) {
  uint8_t p[6];
  p[0]=static_cast<uint8_t>(std::clamp(x,-128,127));
  p[1]=static_cast<uint8_t>(std::clamp(y,-128,127));
  w16(p+2, static_cast<int16_t>(yaw));
  w16(p+4, static_cast<int16_t>(depth));
  return build_frame_hex(config::ADDR_PROPELLER, config::CMD_COMBINED_MOTION, p, 6);
}

std::string pack_servo(int idx, int pwm) {
  uint8_t p[3]; p[0]=idx&0xFF; w16(p+1, static_cast<int16_t>(pwm));
  return build_frame_hex(config::ADDR_SERVO, config::CMD_SET_SERVO_PWM, p, 3);
}

std::string pack_gripper(int dir) {
  int8_t d = (dir>0)?1:((dir<0)?-1:0);
  uint8_t p[1]; p[0]=static_cast<uint8_t>(d);
  return build_frame_hex(config::ADDR_GRIPPER, config::CMD_GRIPPER_SPEED, p, 1);
}

}  // namespace protocol
