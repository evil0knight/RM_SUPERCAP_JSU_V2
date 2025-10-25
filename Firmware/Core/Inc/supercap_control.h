/**
 ******************************************************************************
 * @file           : supercap_control.h
 * @brief          : Supercapacitor control system header
 * @author         : JSU Team
 * @version        : V2.0
 ******************************************************************************
 * @attention
 *
 * This is a replication of HKUST supercapacitor design with improvements:
 * - Back-EMF recovery when wheels stop
 * - Enhanced safety and stability features
 *
 ******************************************************************************
 */

#ifndef __SUPERCAP_CONTROL_H
#define __SUPERCAP_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Configuration Parameters */
#define SUPERCAP_MAX_VOLTAGE        24.0f   // Maximum supercap voltage (V)
#define SUPERCAP_MIN_VOLTAGE        12.0f   // Minimum supercap voltage (V)
#define SUPERCAP_RATED_VOLTAGE      16.0f   // Rated voltage (V)
#define BATTERY_MAX_VOLTAGE         25.2f   // Max battery voltage (V)
#define BATTERY_MIN_VOLTAGE         18.0f   // Min battery voltage (V)
#define MAX_CHARGE_CURRENT          10.0f   // Max charging current (A)
#define MAX_DISCHARGE_CURRENT       20.0f   // Max discharge current (A)
#define BACK_EMF_THRESHOLD          0.5f    // Back-EMF recovery threshold (V)
#define POWER_LIMIT_DEFAULT         80.0f   // Default power limit (W)

/* Safety Thresholds */
#define OVERTEMP_THRESHOLD          75.0f   // Over-temperature threshold (°C)
#define OVERCURRENT_THRESHOLD       25.0f   // Over-current threshold (A)
#define OVERVOLTAGE_THRESHOLD       26.0f   // Over-voltage threshold (V)
#define UNDERVOLTAGE_THRESHOLD      11.0f   // Under-voltage threshold (V)

/* Control Loop Frequencies */
#define CONTROL_LOOP_FREQ_HZ        1000    // Main control loop frequency
#define MONITOR_LOOP_FREQ_HZ        100     // Monitoring loop frequency
#define CAN_TX_FREQ_HZ              50      // CAN transmission frequency

/* Supercapacitor State */
typedef enum {
    SUPERCAP_STATE_INIT = 0,
    SUPERCAP_STATE_CHARGING,
    SUPERCAP_STATE_DISCHARGING,
    SUPERCAP_STATE_IDLE,
    SUPERCAP_STATE_BACK_EMF_RECOVERY,
    SUPERCAP_STATE_ERROR,
    SUPERCAP_STATE_PROTECT
} SupercapState_t;

/* Error Flags */
typedef enum {
    ERROR_NONE              = 0x00,
    ERROR_OVERVOLTAGE       = 0x01,
    ERROR_UNDERVOLTAGE      = 0x02,
    ERROR_OVERCURRENT       = 0x04,
    ERROR_OVERTEMP          = 0x08,
    ERROR_CAN_COMM          = 0x10,
    ERROR_SENSOR_FAULT      = 0x20,
    ERROR_BACK_EMF_FAULT    = 0x40
} ErrorFlags_t;

/* System Data Structure */
typedef struct {
    /* Voltage measurements */
    float supercap_voltage;
    float battery_voltage;
    float output_voltage;
    
    /* Current measurements */
    float charge_current;
    float discharge_current;
    float back_emf_current;
    
    /* Temperature */
    float temperature;
    
    /* Power */
    float input_power;
    float output_power;
    float power_limit;
    
    /* State */
    SupercapState_t state;
    uint32_t error_flags;
    
    /* Control parameters */
    float duty_cycle;
    bool charge_enable;
    bool discharge_enable;
    bool back_emf_recovery_enable;
    
    /* Statistics */
    float energy_stored;        // Joules
    float energy_recovered;     // Joules from back-EMF
    uint32_t runtime_seconds;
    
} SupercapData_t;

/* Public Functions */

/**
 * @brief Initialize supercapacitor control system
 * @return true if successful, false otherwise
 */
bool Supercap_Init(void);

/**
 * @brief Main control loop - should be called at CONTROL_LOOP_FREQ_HZ
 */
void Supercap_ControlLoop(void);

/**
 * @brief Update sensor readings
 */
void Supercap_UpdateSensors(void);

/**
 * @brief Set power limit
 * @param power_limit: Power limit in Watts
 */
void Supercap_SetPowerLimit(float power_limit);

/**
 * @brief Enable/disable back-EMF recovery
 * @param enable: true to enable, false to disable
 */
void Supercap_SetBackEMFRecovery(bool enable);

/**
 * @brief Get current system data
 * @return Pointer to system data structure
 */
const SupercapData_t* Supercap_GetData(void);

/**
 * @brief Get current state
 * @return Current supercapacitor state
 */
SupercapState_t Supercap_GetState(void);

/**
 * @brief Check for errors and handle protection
 */
void Supercap_SafetyCheck(void);

/**
 * @brief Emergency stop - disable all outputs
 */
void Supercap_EmergencyStop(void);

/**
 * @brief Reset error flags
 */
void Supercap_ClearErrors(void);

/**
 * @brief Process back-EMF recovery
 * @return Energy recovered in Joules
 */
float Supercap_ProcessBackEMF(void);

#ifdef __cplusplus
}
#endif

#endif /* __SUPERCAP_CONTROL_H */
