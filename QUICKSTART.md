# Quick Start Guide - RM_SUPERCAP_JSU_V2

## 快速开始指南

### 1. 硬件准备 / Hardware Preparation

#### 需要的组件 / Required Components:
- STM32F103C8T6 开发板 / Development board (Blue Pill)
- 超级电容组 / Supercapacitor bank (50F @ 16V recommended)
- 双向DC-DC转换器 / Bidirectional DC-DC converter
- 电流传感器 / Current sensors (3x)
- CAN收发器 / CAN transceiver (TJA1050)
- 6S LiPo电池 / 6S LiPo battery

#### 连接 / Connections:
```
Battery (+) ──→ [DC-DC] ──→ Supercap (+) ──→ Output (+)
Battery (-) ──→ [Ground] ──→ Supercap (-) ──→ Output (-)
             ↑
             └─ MCU Control
```

### 2. 软件安装 / Software Installation

#### 选项A: STM32CubeIDE (推荐 / Recommended)
```bash
# 1. 安装 STM32CubeIDE
# Download from: https://www.st.com/en/development-tools/stm32cubeide.html

# 2. 导入项目 / Import project
File → Import → Existing Projects into Workspace
Select: RM_SUPERCAP_JSU_V2/Firmware

# 3. 配置目标 / Configure target
Project → Properties → C/C++ Build → Settings
Target: STM32F103C8T6

# 4. 编译 / Build
Project → Build All

# 5. 烧录 / Flash
Run → Debug (F11)
```

#### 选项B: Makefile + ARM GCC
```bash
# 1. 安装工具链 / Install toolchain
sudo apt-get install gcc-arm-none-eabi

# 2. 进入固件目录 / Enter firmware directory
cd Firmware/

# 3. 编译 / Build
make

# 4. 烧录 / Flash (使用 st-flash)
st-flash write build/rm_supercap_v2.bin 0x8000000
```

### 3. 初始配置 / Initial Configuration

#### 编辑配置文件 / Edit configuration file:
```c
// Firmware/Core/Inc/supercap_control.h

// 根据你的硬件调整 / Adjust for your hardware:
#define SUPERCAP_MAX_VOLTAGE        24.0f   // 最大电压
#define MAX_CHARGE_CURRENT          10.0f   // 最大充电电流
#define MAX_DISCHARGE_CURRENT       20.0f   // 最大放电电流
```

### 4. 首次测试 / First Test

#### 安全检查 / Safety Checklist:
- [ ] 所有连接正确 / All connections correct
- [ ] 电压范围正确 / Voltage ranges correct
- [ ] 电流限制设置 / Current limits set
- [ ] 温度传感器工作 / Temperature sensor working
- [ ] CAN总线连接 / CAN bus connected

#### 测试步骤 / Test Steps:

**步骤1: 无负载测试 / Step 1: No-load test**
```bash
# 连接调试器 / Connect debugger
# 观察串口输出 / Watch serial output
# 检查LED状态 / Check LED status
```

**步骤2: 低压测试 / Step 2: Low-voltage test**
```bash
# 使用12V电源 / Use 12V supply
# 不连接超级电容 / No supercap connected
# 验证PWM输出 / Verify PWM output
```

**步骤3: 充电测试 / Step 3: Charging test**
```bash
# 连接超级电容 / Connect supercapitor
# 设置低电流限制 / Set low current limit (2A)
# 观察充电过程 / Monitor charging
```

**步骤4: 放电测试 / Step 4: Discharge test**
```bash
# 连接负载电阻 / Connect load resistor
# 测试放电功能 / Test discharge
# 监控电流和电压 / Monitor current and voltage
```

**步骤5: 反电动势测试 / Step 5: Back-EMF test**
```bash
# 连接电机 / Connect motor
# 加速后急停 / Accelerate then brake
# 观察能量回收 / Watch energy recovery
```

### 5. CAN总线配置 / CAN Bus Configuration

#### 连接CAN / Connect CAN:
```
CAN_H (MCU) ──→ CAN_H (Chassis)
CAN_L (MCU) ──→ CAN_L (Chassis)
GND   (MCU) ──→ GND   (Chassis)
```

#### 测试CAN通信 / Test CAN:
```bash
# 使用CAN分析仪 / Use CAN analyzer
# 查看消息 / Watch messages:
# 0x211: 状态信息 (50Hz) / Status (50Hz)
# 0x212: 诊断信息 (1Hz) / Diagnostics (1Hz)
```

#### 发送命令 / Send commands:
```bash
# 设置功率限制 / Set power limit (80W):
CAN ID: 0x210
Data: [0x20, 0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00]
#      ↑ Power = 800 (80.0W)
#           ↑ Enable: Back-EMF recovery
```

### 6. 监控和调试 / Monitoring and Debug

#### 实时监控 / Real-time monitoring:
```c
// 通过CAN读取 / Read via CAN:
// - 超级电容电压 / Supercap voltage
// - 电池电压 / Battery voltage  
// - 充放电电流 / Charge/discharge current
// - 温度 / Temperature
// - 状态 / State
// - 错误标志 / Error flags
```

#### 调试工具 / Debug tools:
- STM32CubeMonitor (图形界面 / GUI)
- Serial Wire Debug (SWD)
- UART调试输出 / UART debug output
- CAN总线分析仪 / CAN bus analyzer

### 7. 常见问题 / Troubleshooting

#### 问题1: 系统不充电 / System not charging
```
原因 / Cause: 
- 电压设置错误 / Wrong voltage settings
- PWM未输出 / PWM not output
- 电流限制太低 / Current limit too low

解决 / Solution:
- 检查配置 / Check configuration
- 验证PWM / Verify PWM
- 增加电流限制 / Increase current limit
```

#### 问题2: 过流保护触发 / Overcurrent protection triggered
```
原因 / Cause:
- 负载太大 / Load too high
- 短路 / Short circuit
- 传感器故障 / Sensor fault

解决 / Solution:
- 减小负载 / Reduce load
- 检查连接 / Check connections
- 校准传感器 / Calibrate sensors
```

#### 问题3: CAN通信失败 / CAN communication failed
```
原因 / Cause:
- 波特率不匹配 / Baud rate mismatch
- 终端电阻缺失 / Missing termination
- 接线错误 / Wrong wiring

解决 / Solution:
- 设置为1Mbps / Set to 1Mbps
- 添加120Ω电阻 / Add 120Ω resistor
- 检查H/L接线 / Check H/L wiring
```

#### 问题4: 反电动势不工作 / Back-EMF not working
```
原因 / Cause:
- 阈值太高 / Threshold too high
- 电机未连接 / Motor not connected
- 电路故障 / Circuit fault

解决 / Solution:
- 降低阈值 / Lower threshold
- 检查电机连接 / Check motor connection
- 验证整流电路 / Verify rectifier circuit
```

### 8. 性能优化 / Performance Optimization

#### 调整控制参数 / Tune control parameters:
```c
// In supercap_control.c

// 充电速度 / Charging speed
float duty_cycle += current_error * 0.01f;  // 增加→更快 / Increase = faster

// 反电动势灵敏度 / Back-EMF sensitivity  
#define BACK_EMF_THRESHOLD  0.3f  // 降低→更灵敏 / Lower = more sensitive
```

#### 监控性能 / Monitor performance:
- 充电效率 / Charging efficiency: >95%
- 放电效率 / Discharge efficiency: >95%
- 反电动势回收 / Back-EMF recovery: 5-15%
- 响应时间 / Response time: <10ms

### 9. 维护 / Maintenance

#### 定期检查 / Regular checks:
- [ ] 清洁连接器 / Clean connectors
- [ ] 检查螺丝 / Check screws
- [ ] 测试传感器 / Test sensors
- [ ] 校准电压/电流 / Calibrate voltage/current
- [ ] 更新固件 / Update firmware

#### 超级电容维护 / Supercap maintenance:
- 避免过压 / Avoid overvoltage
- 避免过热 / Avoid overheating
- 定期充放电 / Regular charge/discharge
- 检查ESR / Check ESR

### 10. 安全注意事项 / Safety Warnings

⚠️ **危险 / DANGER:**
- 高电压和大电流 / High voltage and current
- 超级电容保持电荷 / Supercaps hold charge
- 短路会引起火灾 / Short circuit risk

🛡️ **安全措施 / Safety measures:**
- 总是先断电 / Always power off first
- 使用绝缘工具 / Use insulated tools
- 戴安全眼镜 / Wear safety glasses
- 在通风处工作 / Work in ventilated area
- 准备灭火器 / Have fire extinguisher ready

### 11. 获取帮助 / Getting Help

📚 **文档 / Documentation:**
- [详细技术文档](Docs/README_DETAILED.md)
- [硬件设计指南](Hardware/HARDWARE_DESIGN.md)
- [配置指南](Docs/CONFIGURATION.md)

💬 **支持 / Support:**
- GitHub Issues
- RoboMaster论坛 / RoboMaster Forum
- 开发团队 / Development Team

### 12. 下一步 / Next Steps

完成基础测试后 / After basic testing:
1. 集成到机器人 / Integrate into robot
2. 调整功率限制 / Tune power limits
3. 优化性能 / Optimize performance
4. 记录数据 / Log data
5. 参加比赛 / Compete!

---

**祝你好运！ / Good luck!** 🚀

如有问题，请查看详细文档或提交Issue。
For questions, see detailed docs or open an issue.
