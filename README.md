# RM_SUPERCAP_JSU_V2

RoboMaster 超级电容系统 V2.0

## 项目简介 / Project Overview

复刻香港科技大学的超级电容设计，相比上一版本的优势：
- ✅ 可以回收轮子停下的反电动势（能量回收）
- ✅ 整体更安全稳定

Replication of HKUST supercapacitor design with improvements:
- ✅ Back-EMF recovery when wheels stop (energy recovery)
- ✅ Overall safer and more stable

## 主要特性 / Key Features

### 核心功能 / Core Functions
- 智能充放电控制 / Intelligent charge/discharge control
- **反电动势能量回收** / **Back-EMF energy recovery** (NEW!)
- 功率限制管理 / Power limiting
- 实时监控 / Real-time monitoring

### 安全特性 / Safety Features
- 过压/欠压保护 / Over/under voltage protection
- 过流保护 / Overcurrent protection
- 过温保护 / Overtemperature protection
- 紧急停止 / Emergency stop

### 通信 / Communication
- CAN总线接口 / CAN bus interface
- RoboMaster协议兼容 / RoboMaster protocol compatible

## 快速开始 / Quick Start

### 目录结构 / Directory Structure
```
RM_SUPERCAP_JSU_V2/
├── Firmware/          # 固件源代码 / Firmware source code
│   └── Core/
│       ├── Inc/       # 头文件 / Header files
│       └── Src/       # 源文件 / Source files
├── Hardware/          # 硬件设计文件 / Hardware design files
│   ├── Schematics/    # 原理图 / Schematics
│   └── PCB/           # PCB设计 / PCB designs
└── Docs/              # 文档 / Documentation

```

### 文档 / Documentation
详细文档请查看: See detailed documentation:
- [完整技术文档 / Full Technical Documentation](Docs/README_DETAILED.md)

## 技术规格 / Technical Specifications

- 超级电容电压范围 / Supercap voltage: 12V - 24V
- 额定工作电压 / Rated voltage: 16V
- 最大充电电流 / Max charge current: 10A
- 最大放电电流 / Max discharge current: 20A
- 控制频率 / Control frequency: 1000Hz
- CAN通信速率 / CAN speed: 1Mbps

## 能量回收系统 / Energy Recovery System

当机器人轮子减速时，电机产生反电动势。本系统捕获这部分能量并储存到超级电容中，提高整体能效5-15%。

When robot wheels decelerate, motors generate back-EMF. This system captures that energy and stores it in the supercapacitor, improving overall efficiency by 5-15%.

## 版本对比 / Version Comparison

| 特性 / Feature | V1 | V2 |
|----------------|----|----|
| 反电动势回收 / Back-EMF Recovery | ❌ | ✅ |
| 安全保护 / Safety Features | 基础 / Basic | 增强 / Enhanced |
| 能量监控 / Energy Monitoring | ❌ | ✅ |
| 诊断功能 / Diagnostics | ❌ | ✅ |

## 贡献 / Contributing

欢迎提交问题和改进建议！
Issues and improvements are welcome!

## 致谢 / Credits

- 原始设计灵感 / Original design: HKUST RoboMaster Team
- V2改进与实现 / V2 improvements: JSU Team

## 许可 / License

Open source for educational and RoboMaster competition use.
