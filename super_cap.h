#ifndef SUPER_CAP_H
#define SUPER_CAP_H
#include "main.h"
#include "stm32f4xx_it.h"
#include "user_lib.h"
#include "struct_typedef.h"

// 错误代码定义 (errorCode的bit0-6)
#define SUPERCAP_ERROR_UNDER_VOLTAGE      0x01  // Bit 0: 欠压
#define SUPERCAP_ERROR_OVER_VOLTAGE       0x02  // Bit 1: 过压
#define SUPERCAP_ERROR_BUCK_BOOST         0x04  // Bit 2: Buck-Boost转换器错误
#define SUPERCAP_ERROR_SHORT_CIRCUIT      0x08  // Bit 3: 短路
#define SUPERCAP_ERROR_HIGH_TEMPERATURE   0x10  // Bit 4: 高温
#define SUPERCAP_ERROR_NO_POWER_INPUT     0x20  // Bit 5: 无电源输入
#define SUPERCAP_ERROR_CAPACITOR          0x40  // Bit 6: 电容故障

// 默认配置参数
#define SUPERCAP_DEFAULT_POWER_LIMIT      37    // 默认功率限制 (W)
#define SUPERCAP_DEFAULT_ENERGY_BUFFER    60    // 默认能量缓冲 (J)

// 接收数据结构 (从超电板接收, CAN ID: 0x051)
typedef struct
{
    uint8_t errorCode;           // 错误代码 (bit7=输出禁用, bit0-6=错误码)
    float chassisPower;          // 底盘功率 (单位: W)
    uint16_t chassisPowerLimit;  // 底盘功率限制值 (单位: W)
    uint8_t capEnergy;           // 电容能量 (0-255 对应 0-100%)
} __attribute__((packed)) SuperCap_Msg_get;

// 发送数据结构 (发送给超电板, CAN ID: 0x061)
typedef struct
{
    uint8_t enableDCDC : 1;      // bit0: DCDC使能标志 (1=使能, 0=禁用)
    uint8_t systemRestart : 1;   // bit1: 系统重启命令 (1=重启)
    uint8_t resv0 : 6;           // bit2-7: 保留位
    uint16_t powerLimit;         // 功率限制值 (单位: W, 范围: 30-250W)
    uint16_t energyBuffer;       // 能量缓冲值 (单位: J, 范围: 0-300J)
    uint8_t resv1[3];            // 3字节保留位
} __attribute__((packed)) SuperCap_TX_Msg_send;

// 辅助函数：获取输出禁用状态
#define SUPERCAP_OUTPUT_DISABLED(errorCode) (((errorCode) >> 7) & 0x01)
// 辅助函数：获取错误码
#define SUPERCAP_GET_ERROR(errorCode) ((errorCode) & 0x7F)
// 辅助函数：计算电容能量百分比
#define SUPERCAP_ENERGY_PERCENT(capEnergy) ((capEnergy) * 100.0f / 255.0f)

extern uint8_t get_supercap_online_state(void);

extern void get_supercap(SuperCap_Msg_get *cap, uint8_t *data);

/**
 * @brief 设置超电控制参数
 *
 * @param cap 超电发送实例
 * @param enable DCDC使能 (1=使能, 0=禁用)
 * @param power_limit 功率限制 (30-250W)
 * @param energy_buffer 能量缓冲 (0-300J)
 */
extern void SuperCapSetControl(SuperCap_TX_Msg_send *cap, uint8_t enable, uint16_t power_limit, uint16_t energy_buffer);

/**
 * @brief 使能超电DCDC
 *
 * @param cap 超电发送实例
 */
extern void SuperCapEnable(SuperCap_TX_Msg_send *cap);

/**
 * @brief 禁用超电DCDC
 *
 * @param cap 超电发送实例
 */
extern void SuperCapDisable(SuperCap_TX_Msg_send *cap);

/**
 * @brief 设置功率限制
 *
 * @param cap 超电发送实例
 * @param power_limit 功率限制值 (30-250W)
 */
extern void SuperCapSetPowerLimit(SuperCap_TX_Msg_send *cap, uint16_t power_limit);

/**
 * @brief 设置能量缓冲
 *
 * @param cap 超电发送实例
 * @param energy_buffer 能量缓冲值 (0-300J)
 */
extern void SuperCapSetEnergyBuffer(SuperCap_TX_Msg_send *cap, uint16_t energy_buffer);

/**
 * @brief 发送系统重启命令
 *
 * @param cap 超电发送实例
 */
extern void SuperCapSystemRestart(SuperCap_TX_Msg_send *cap);

/**
 * @brief 获取超电是否输出禁用
 *
 * @param cap 超电接收实例
 * @return 1=输出禁用, 0=输出使能
 */
extern uint8_t SuperCapIsOutputDisabled(SuperCap_Msg_get *cap);

/**
 * @brief 获取超电能量百分比
 *
 * @param cap 超电接收实例
 * @return 能量百分比 (0-100)
 */
extern float SuperCapGetEnergyPercent(SuperCap_Msg_get *cap);

#endif // !SUPER_CAP_H
