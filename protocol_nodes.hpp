/*******************************************************************************
 * protocol_nodes.hpp — 四个 Encoder，标量输入 → AA55 hex 输出
 ******************************************************************************/

#pragma once

#include <fins/node.hpp>
#include "protocol.h"

// ──────────── MotionEncoder ────────────
class MotionEncoder : public fins::Node {
public:
  void define() override {
    set_name("MotionEncoder");
    set_description("x/y/yaw/depth → AA55 motion 帧 hex");
    set_category("Protocol of ROV");
    register_input<int>("x", &MotionEncoder::on_x);
    register_input<int>("y", &MotionEncoder::on_y);
    register_input<int>("yaw", &MotionEncoder::on_yaw);
    register_input<int>("depth", &MotionEncoder::on_depth);
    register_output<std::string>("hex");
  }
  void on_x(const int &v, fins::AcqTime)     { x_=v; mask_|=1; try_send(); }
  void on_y(const int &v, fins::AcqTime)     { y_=v; mask_|=2; try_send(); }
  void on_yaw(const int &v, fins::AcqTime)   { yaw_=v; mask_|=4; try_send(); }
  void on_depth(const int &v, fins::AcqTime) { depth_=v; mask_|=8; try_send(); }
private:
  int x_=0, y_=0, yaw_=0, depth_=0, mask_=0;
  void try_send() {
    if (mask_ == 15) {
      send("hex", protocol::pack_motion(x_, y_, yaw_, depth_));
      mask_ = 0;
    }
  }
};
EXPORT_NODE(MotionEncoder)

// ──────────── ServoXEncoder ────────────
class ServoXEncoder : public fins::Node {
public:
  void define() override {
    set_name("ServoXEncoder");
    set_description("pwm_x → AA55 servo(1) 帧 hex");
    set_category("Protocol of ROV");
    register_input<int>("pwm_x", &ServoXEncoder::on_pwm_x);
    register_output<std::string>("hex");
  }
  void on_pwm_x(const int &v, fins::AcqTime) {
    send("hex", protocol::pack_servo(1, v));
  }
};
EXPORT_NODE(ServoXEncoder)

// ──────────── ServoYEncoder ────────────
class ServoYEncoder : public fins::Node {
public:
  void define() override {
    set_name("ServoYEncoder");
    set_description("pwm_y → AA55 servo(2) 帧 hex");
    set_category("Protocol of ROV");
    register_input<int>("pwm_y", &ServoYEncoder::on_pwm_y);
    register_output<std::string>("hex");
  }
  void on_pwm_y(const int &v, fins::AcqTime) {
    send("hex", protocol::pack_servo(2, v));
  }
};
EXPORT_NODE(ServoYEncoder)

// ──────────── GripperEncoder ────────────
class GripperEncoder : public fins::Node {
public:
  void define() override {
    set_name("GripperEncoder");
    set_description("direction → AA55 gripper 帧 hex");
    set_category("Protocol of ROV");
    register_input<int>("direction", &GripperEncoder::on_direction);
    register_output<std::string>("hex");
  }
  void on_direction(const int &dir, fins::AcqTime) {
    send("hex", protocol::pack_gripper(dir));
  }
};
EXPORT_NODE(GripperEncoder)
