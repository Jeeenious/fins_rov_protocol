# fins_rov_proto — AA55 协议编码插件

四个 Encoder 节点，标量输入 → AA55 hex 输出。

## 节点

| 节点 | 输入端口 | 类型 | 输出 |
|------|---------|------|------|
| MotionEncoder | x, y, yaw, depth | int | hex (mask=15 合批) |
| ServoXEncoder | pwm_x | int | hex |
| ServoYEncoder | pwm_y | int | hex |
| GripperEncoder | direction | int | hex |

## AA55 帧结构

```
| 0xAA | 0x55 | addr | cmd | plen | payload[plen] | crc_lo | crc_hi |
  ↑ 帧头         ↑ 帧体(3+plen bytes)             ↑ CRC16-Modbus(帧体)
```

- **plen** ≤ 255，帧总长 = 7 + plen bytes
- **CRC** 使用 Modbus CRC-16（多项式 0xA001），对帧体 `[addr, cmd, plen, payload...]` 校验

## 帧类型

| 命令 | addr | cmd | payload | 帧长 |
|------|------|-----|---------|------|
| motion | 0x02 | 0x31 | x(i8) y(i8) yaw(i16le) depth(i16le) | 11 |
| servo | 0x03 | 0x03 | idx(u8) pwm(i16le) | 8 |
| gripper | 0x05 | 0x00 | dir(i8) | 6 |

## 编码 API

```cpp
std::string protocol::pack_motion(int x, int y, int yaw, int depth);
std::string protocol::pack_servo(int idx, int pwm);
std::string protocol::pack_gripper(int dir);
// 均返回小写 hex 字符串，调用方通过 TcpProxySender.send_hex() 下发
```

## 管线

```
solver (int) → Encoder → hex → TcpProxySender → TCP → PI
```
