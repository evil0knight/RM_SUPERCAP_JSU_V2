# STM32 超级电容控制器 - 编译与烧录指南

本文档说明如何在 Windows 环境下编译和烧录 STM32F334R8 超级电容控制器固件。

---

## 📋 环境要求

### 必需工具

- **ARM GCC 工具链**: arm-none-eabi-gcc
- **Make**: GNU Make 4.0+
- **STM32CubeProgrammer**: ST 官方烧录工具
- **ST-Link**: ST-Link V2/V3 调试器

### 验证工具安装

在 PowerShell 或 CMD 中运行以下命令验证：

```powershell
# 检查 GCC 工具链
arm-none-eabi-gcc --version

# 检查 Make
make --version

# 检查 STM32CubeProgrammer
STM32_Programmer_CLI.exe --version
```

---

## 🔨 编译固件

### 1. 进入项目目录

```powershell
cd "d:\RM\xiang_gang_ke_ji\NEWSuperCapacitorController-master\NEWSuperCapacitorController-master"
```

### 2. 执行编译

```powershell
# 编译固件（HARDWARE_ID 必须指定，根据实际硬件修改）
make -j HARDWARE_ID=101
```

**参数说明：**

- `-j`: 并行编译，利用多核 CPU 加速
- `HARDWARE_ID=101`: 硬件 ID（**必需参数**），根据你的实际硬件修改

### 3. 编译输出

编译成功后，在 `build/` 目录生成以下文件：

- `RM2024-SuperCap-F3-V1R.elf` - ELF 格式（用于调试）
- `RM2024-SuperCap-F3-V1R.hex` - HEX 格式（用于烧录）
- `RM2024-SuperCap-F3-V1R.bin` - BIN 格式（用于烧录）

### 4. 查看内存使用情况

```powershell
make size
```

输出示例：

```
   text    data     bss     dec     hex filename
  23432     176    2920   26528    67a0 build/RM2024-SuperCap-F3-V1R.elf
```

- **text**: 代码段大小
- **data**: 已初始化数据
- **bss**: 未初始化数据
- **dec**: 总大小（十进制）

### 5. 清理编译文件

```powershell
# 清理所有编译产物
make clean

# 重新编译（清理 + 编译）
make rebuild HARDWARE_ID=101
```

---

## 🔥 烧录固件（命令行方式）

### 硬件准备

1. **连接 ST-Link 到 STM32 板子**：

   - SWDIO → STM32 SWDIO
   - SWCLK → STM32 SWCLK
   - GND → GND
   - 3.3V → 3.3V（如需供电）
2. **给板子供电**
3. **ST-Link 连接到电脑 USB 口**

### 烧录命令

```powershell
# 使用 STM32CubeProgrammer 命令行工具烧录
STM32_Programmer_CLI.exe -c port=SWD -e all -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst
```

**参数说明：**

- `-c port=SWD`: 使用 SWD 接口连接
- `-e all`: 擦除整个芯片
- `-d build/RM2024-SuperCap-F3-V1R.hex`: 下载的固件文件
- `-v`: 验证烧录
- `-hardRst`: 烧录后硬件复位

### 烧录成功输出示例

```
      -------------------------------------------------------------------
                       STM32CubeProgrammer v2.x.x
      -------------------------------------------------------------------

=====  DFU/USB connection =====
...
Memory Programming ...
Opening and parsing file: RM2024-SuperCap-F3-V1R.hex
  File          : RM2024-SuperCap-F3-V1R.hex
  Size          : 23.6 KB
  Address       : 0x08000000

Erasing memory corresponding to segment 0:
Erasing internal memory sector 0
Download in Progress:
[==================================================] 100%

File download complete
Time elapsed during download operation: 00:00:02.345

RUNNING Program ...
  Address:      : 0x8000000
Start operation achieved successfully
```

---

## 🔥 烧录固件（Ozone + JLink 方式，推荐用于调试）

### 优势

- ⭐⭐⭐⭐⭐ 稳定性最好
- ⭐⭐⭐⭐⭐ 功能最强大（实时跟踪、性能分析、波形图）
- ⭐⭐⭐⭐⭐ 界面友好，专业调试工具

### 硬件准备

1. **连接 JLink 到 STM32 板子**：

   - SWDIO → STM32 SWDIO
   - SWCLK → STM32 SWCLK
   - GND → GND
   - VTref → 3.3V（用于检测目标电压）
2. **给板子供电**
3. **JLink 连接到电脑 USB 口**

### 使用 Ozone 下载和调试

#### 方法1：使用项目文件（推荐）

1. **双击打开项目文件**：

   ```
   SuperCap-F334.jdebug
   ```
2. **Ozone 会自动**：

   - 连接到 JLink 和 STM32F334R8
   - 加载 ELF 文件
3. **下载程序**：

   - 按 **F5** 或点击 "Download & Reset Program"
   - 固件自动烧录并运行

#### 方法2：手动创建项目

如果没有 `.jdebug` 文件：

1. 启动 Ozone，创建新项目
2. **Device**: STM32F334R8
3. **Target Interface**: SWD
4. **Host Interface**: USB
5. **Program File**: 选择 `build/RM2024-SuperCap-F3-V1R.elf`
6. 按 **F5** 下载程序

### Ozone 调试操作

#### 基本操作

- **F5**: 下载并复位程序
- **F9**: 继续运行（Go）
- **F8**: 暂停
- **F10**: 单步跳过（Step Over）
- **F11**: 单步进入（Step Into）
- **Shift+F11**: 跳出函数（Step Out）

#### 设置断点

- 双击源代码行号左侧设置断点（红点）
- 右键 → Toggle Breakpoint

#### 查看变量

- **Watched Data**: 添加监视表达式
  ```
  PowerManager::processedData.voltage_a
  PowerManager::status.errorCode
  ```
- **Locals**: 自动显示局部变量
- **Registers**: 查看 CPU 寄存器

#### 实时数据采样（高级功能）

1. View → Data Sampling
2. 添加要采样的变量
3. 点击 Start
4. 实时查看变量波形图！

### 修正源文件路径

如果 Ozone 找不到源文件：

1. 双击左侧 Source Files 中的文件
2. 浏览到正确位置：
   ```
   d:\RM\xiang_gang_ke_ji\NEWSuperCapacitorController-master\NEWSuperCapacitorController-master\Core\Src\
   ```
3. 选择对应的源文件

或者重新编译固件，确保路径正确。

---

## 🎯 完整流程（一键编译+烧录）

```powershell
# 1. 进入项目目录
cd "d:\RM\xiang_gang_ke_ji\NEWSuperCapacitorController-master\NEWSuperCapacitorController-master"

# 2. 清理并重新编译
make clean
make -j HARDWARE_ID=101

# 3. 烧录固件
STM32_Programmer_CLI.exe -c port=SWD -e all -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst
```

---

## ❓ 常见问题

### 问题1: 编译时出现警告 `process_begin: CreateProcess(NULL, expr + 1, ...) failed`

**原因**: Makefile 使用了 Linux shell 命令（expr、echo -e），Windows 环境不支持

**影响**: 仅影响编译进度显示，不影响编译结果

**解决**: 忽略即可，或安装 MSYS2 完整环境

---

### 问题2: 烧录失败 `Error: No ST-LINK detected`

**原因**: ST-Link 未连接或驱动未安装

**解决步骤**:

1. 检查 ST-Link 是否插入电脑
2. 检查设备管理器中 ST-Link 驱动状态
3. 重新安装 ST-Link 驱动（STM32CubeProgrammer 会自动安装）
4. 尝试更换 USB 口

---

### 问题3: 烧录失败 `Error: Target not found`

**原因**: 无法连接到目标芯片

**解决步骤**:

1. 检查 SWDIO、SWCLK、GND 连接
2. 确认板子已供电（目标电压应显示约 3.3V）
3. 尝试使用 `mode=UR` 参数（Under Reset 模式）：
   ```powershell
   STM32_Programmer_CLI.exe -c port=SWD mode=UR -e all -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst
   ```

---

### 问题4: 烧录失败 `Error: Flash operation failed`

**原因**: Flash 写保护或读保护

**解决步骤**:

```powershell
# 1. 解除保护（会擦除整个芯片）
STM32_Programmer_CLI.exe -c port=SWD -ob RDP=0xAA

# 2. 重新连接
STM32_Programmer_CLI.exe -c port=SWD -e all

# 3. 烧录固件
STM32_Programmer_CLI.exe -c port=SWD -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst
```

---

### 问题5: 烧录后蜂鸣器持续响（1000Hz）

**原因**: HARDWARE_ID 不匹配

**说明**: 固件启动时会检查 Flash Option Bytes 中存储的 HARDWARE_ID 是否与编译时指定的 ID 匹配

**解决**:

1. 确认编译时使用的 HARDWARE_ID（如 `make HARDWARE_ID=101`）
2. 或在校准模式下重新烧录（固件会自动写入 HARDWARE_ID）

---

## 📊 内存限制

**STM32F334R8 规格：**

- Flash: 64 KB
- RAM: 12 KB

**当前固件占用（参考）：**

- Flash: ~23.6 KB (37%)
- RAM: ~3 KB (25%)

---

## 🔧 高级选项

### 仅擦除芯片

```powershell
STM32_Programmer_CLI.exe -c port=SWD -e all
```

### 读取芯片内容

```powershell
STM32_Programmer_CLI.exe -c port=SWD -r firmware_backup.hex 0x08000000 0x10000
```

### 验证固件

```powershell
STM32_Programmer_CLI.exe -c port=SWD -v build/RM2024-SuperCap-F3-V1R.hex
```

### 查看 Option Bytes

```powershell
STM32_Programmer_CLI.exe -c port=SWD -ob displ
```

---

## 📚 参考资料

- [STM32CubeProgrammer 用户手册](https://www.st.com/resource/en/user_manual/um2237-stm32cubeprogrammer-software-description-stmicroelectronics.pdf)
- [STM32F334 数据手册](https://www.st.com/resource/en/datasheet/stm32f334r8.pdf)
- [项目说明文档](CLAUDE.md)

---

## ✅ 快速参考

### 编译命令

```powershell
make -j HARDWARE_ID=101        # 编译
make clean                      # 清理
make rebuild HARDWARE_ID=101   # 重新编译
make size                       # 查看大小
```

### 烧录命令

```powershell
# 标准烧录
STM32_Programmer_CLI.exe -c port=SWD -e all -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst

# Under Reset 模式烧录
STM32_Programmer_CLI.exe -c port=SWD mode=UR -e all -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst

# 不擦除烧录（更快，但不推荐）
STM32_Programmer_CLI.exe -c port=SWD -d build/RM2024-SuperCap-F3-V1R.hex -v -hardRst
```

---

**最后更新**: 2025-10-29
**适用版本**: RM2024-SuperCap-F3-V1R
