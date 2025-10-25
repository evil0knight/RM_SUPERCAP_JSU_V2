/**
 ******************************************************************************
 * @file           : can_interface.h
 * @brief          : CAN communication interface for supercapacitor
 * @author         : JSU Team
 * @version        : V2.0
 ******************************************************************************
 */

#ifndef __CAN_INTERFACE_H
#define __CAN_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* CAN IDs for RoboMaster Protocol */
#define CAN_SUPERCAP_TX_ID          0x211   // Supercap status to chassis
#define CAN_SUPERCAP_RX_ID          0x210   // Commands from chassis
#define CAN_SUPERCAP_DIAG_ID        0x212   // Diagnostic data

/* CAN Message Structures */

// TX: Supercap status (0x211)
typedef struct __attribute__((packed)) {
    uint16_t supercap_voltage;      // Voltage * 100 (e.g., 1650 = 16.50V)
    uint16_t battery_voltage;       // Voltage * 100
    uint16_t output_current;        // Current * 100 (A)
    uint8_t state;                  // Current state
    uint8_t error_flags;            // Error flags
} CAN_SupercapStatus_t;

// RX: Commands from chassis (0x210)
typedef struct __attribute__((packed)) {
    uint16_t power_limit;           // Power limit * 10 (W)
    uint8_t enable_flags;           // Enable flags
    uint8_t reserved[5];
} CAN_ChassisCommand_t;

// TX: Diagnostic data (0x212)
typedef struct __attribute__((packed)) {
    int16_t temperature;            // Temperature * 10 (°C)
    uint16_t energy_stored;         // Energy * 10 (J)
    uint16_t energy_recovered;      // Recovered energy * 10 (J)
    uint16_t runtime_minutes;       // Runtime in minutes
} CAN_DiagnosticData_t;

/* Enable Flags */
#define CAN_ENABLE_CHARGE           0x01
#define CAN_ENABLE_DISCHARGE        0x02
#define CAN_ENABLE_BACK_EMF         0x04
#define CAN_RESET_ERRORS            0x08

/* Public Functions */

/**
 * @brief Initialize CAN interface
 * @return true if successful
 */
bool CAN_Interface_Init(void);

/**
 * @brief Send supercap status via CAN
 */
void CAN_SendSupercapStatus(void);

/**
 * @brief Send diagnostic data via CAN
 */
void CAN_SendDiagnosticData(void);

/**
 * @brief Process received CAN messages
 */
void CAN_ProcessReceivedMessages(void);

/**
 * @brief CAN receive callback
 * @param rx_data: Received data buffer
 * @param can_id: CAN message ID
 * @param dlc: Data length code
 */
void CAN_RxCallback(uint8_t *rx_data, uint32_t can_id, uint8_t dlc);

#ifdef __cplusplus
}
#endif

#endif /* __CAN_INTERFACE_H */
