/**
 * @file super_cap.c
 * @brief 超级电�?�通信模块 - 适配新协�?
 * @note CAN ID: 接收0x051, 发�?0x061
 */

#include "super_cap.h"
#include "stdlib.h"
#include "main.h"
#include "stm32f4xx_it.h"
#include "user_lib.h"
#include "struct_typedef.h"
#include "CAN_receive.h"
#include "referee.h"
#include <string.h>

uint32_t LastCapTick = 0;  // 上一次收到超电信号的时间�?
uint32_t NowCapTick = 0;   // �?次收到超电信号的时间�?

/**
 * @brief 获取超级电�?�在线状�?
 * @retval 1=在线, 0=离线
 */
uint8_t get_supercap_online_state(void)
{
    NowCapTick = HAL_GetTick();
    uint32_t DeltaCapTick = NowCapTick - LastCapTick;

    if (DeltaCapTick > 1000) {
        // 超过1s�?收到数据，�?�线
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief 解析超电板返回数�?
 * @param cap 超电接收数据结构
 * @param data CAN接收的原始数�? (8字节)
 * @note 数据格式:
 *       Byte 0: errorCode (bit7=输出禁用, bit0-6=错�??�?)
 *       Byte 1-4: chassisPower (float, IEEE 754, 小�??�?)
 *       Byte 5-6: chassisPowerLimit (uint16, 小�??�?)
 *       Byte 7: capEnergy (0-255)
 */
void get_supercap(SuperCap_Msg_get *cap, uint8_t *data)
{
    LastCapTick = HAL_GetTick();

    // 直接按协�?格式解析
    cap->errorCode = data[0];

    // 解析float (小�??�?, IEEE 754) - 使用联合体避免packed对齐�?�?
    union {
        float f;
        uint8_t bytes[4];
    } float_converter;

    float_converter.bytes[0] = data[1];
    float_converter.bytes[1] = data[2];
    float_converter.bytes[2] = data[3];
    float_converter.bytes[3] = data[4];
    cap->chassisPower = float_converter.f;

    // 解析uint16 (小�??�?)
    cap->chassisPowerLimit = (uint16_t)data[5] | ((uint16_t)data[6] << 8);

    // 电�?�能�?
    cap->capEnergy = data[7];
}

/**
 * @brief 设置超电完整控制参数
 */
void SuperCapSetControl(SuperCap_TX_Msg_send *cap, uint8_t enable, uint16_t power_limit, uint16_t energy_buffer)
{
    cap->enableDCDC = enable ? 1 : 0;
    cap->systemRestart = 0;
    cap->resv0 = 0;
    cap->powerLimit = power_limit;
    cap->energyBuffer = energy_buffer;
    cap->resv1[0] = 0;
    cap->resv1[1] = 0;
    cap->resv1[2] = 0;
}

/**
 * @brief 设置功率限制
 */
void SuperCapSetPowerLimit(SuperCap_TX_Msg_send *cap, uint16_t power_limit)
{
    cap->powerLimit = power_limit;
}

/**
 * @brief 设置能量缓冲
 */
void SuperCapSetEnergyBuffer(SuperCap_TX_Msg_send *cap, uint16_t energy_buffer)
{
    cap->energyBuffer = energy_buffer;
}

/**
 * @brief 使能超电DCDC
 */
void SuperCapEnable(SuperCap_TX_Msg_send *cap)
{
    cap->enableDCDC = 1;
    cap->systemRestart = 0;  // �?保不触发重启
}

/**
 * @brief 禁用超电DCDC
 */
void SuperCapDisable(SuperCap_TX_Msg_send *cap)
{
    cap->enableDCDC = 0;
    cap->systemRestart = 0;  // �?保不触发重启
}

/**
 * @brief 发送系统重�?命令
 */
void SuperCapSystemRestart(SuperCap_TX_Msg_send *cap)
{
    cap->systemRestart = 1;
}

/**
 * @brief 获取超电�?否输出�?�用
 * @return 1=输出禁用, 0=输出使能
 */
uint8_t SuperCapIsOutputDisabled(SuperCap_Msg_get *cap)
{
    return (cap->errorCode >> 7) & 0x01;
}

/**
 * @brief 获取超电能量百分�?
 * @return 能量百分�? (0.0 - 100.0)
 */
float SuperCapGetEnergyPercent(SuperCap_Msg_get *cap)
{
    return (float)cap->capEnergy * 100.0f / 255.0f;
}
