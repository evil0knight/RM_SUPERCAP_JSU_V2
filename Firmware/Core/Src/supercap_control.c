/**
 ******************************************************************************
 * @file           : supercap_control.c
 * @brief          : Supercapacitor control system implementation
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

#include "supercap_control.h"
#include <math.h>
#include <string.h>

/* Private variables */
static SupercapData_t supercap_data = {0};
static uint32_t tick_counter = 0;

/* Private function prototypes */
static void Supercap_StateManager(void);
static void Supercap_ChargingControl(void);
static void Supercap_DischargingControl(void);
static void Supercap_BackEMFRecoveryControl(void);
static float Supercap_CalculateEnergy(float voltage);
static void Supercap_UpdatePWM(float duty_cycle);

/**
 * @brief Initialize supercapacitor control system
 */
bool Supercap_Init(void)
{
    memset(&supercap_data, 0, sizeof(SupercapData_t));
    
    supercap_data.state = SUPERCAP_STATE_INIT;
    supercap_data.power_limit = POWER_LIMIT_DEFAULT;
    supercap_data.back_emf_recovery_enable = true;
    supercap_data.charge_enable = false;
    supercap_data.discharge_enable = false;
    
    // Initialize PWM at 0% duty cycle
    Supercap_UpdatePWM(0.0f);
    
    // Perform initial sensor read
    Supercap_UpdateSensors();
    
    // Check initial safety conditions
    Supercap_SafetyCheck();
    
    if (supercap_data.error_flags == ERROR_NONE) {
        supercap_data.state = SUPERCAP_STATE_IDLE;
        return true;
    }
    
    return false;
}

/**
 * @brief Update sensor readings
 */
void Supercap_UpdateSensors(void)
{
    // TODO: Implement actual ADC readings for hardware
    // These are placeholder implementations
    
    // Read voltages (implement with actual ADC)
    // supercap_data.supercap_voltage = ADC_ReadSupercapVoltage();
    // supercap_data.battery_voltage = ADC_ReadBatteryVoltage();
    // supercap_data.output_voltage = ADC_ReadOutputVoltage();
    
    // Read currents (implement with current sensors)
    // supercap_data.charge_current = ADC_ReadChargeCurrent();
    // supercap_data.discharge_current = ADC_ReadDischargeCurrent();
    // supercap_data.back_emf_current = ADC_ReadBackEMFCurrent();
    
    // Read temperature (implement with temp sensor)
    // supercap_data.temperature = ADC_ReadTemperature();
    
    // Calculate power
    supercap_data.input_power = supercap_data.battery_voltage * supercap_data.charge_current;
    supercap_data.output_power = supercap_data.output_voltage * supercap_data.discharge_current;
    
    // Calculate stored energy
    supercap_data.energy_stored = Supercap_CalculateEnergy(supercap_data.supercap_voltage);
}

/**
 * @brief Main control loop
 */
void Supercap_ControlLoop(void)
{
    tick_counter++;
    
    // Update sensors every cycle
    Supercap_UpdateSensors();
    
    // Safety check every cycle
    Supercap_SafetyCheck();
    
    if (supercap_data.state == SUPERCAP_STATE_ERROR || 
        supercap_data.state == SUPERCAP_STATE_PROTECT) {
        Supercap_EmergencyStop();
        return;
    }
    
    // State management and control
    Supercap_StateManager();
    
    // Update runtime statistics every second
    if (tick_counter % CONTROL_LOOP_FREQ_HZ == 0) {
        supercap_data.runtime_seconds++;
    }
}

/**
 * @brief State management logic
 */
static void Supercap_StateManager(void)
{
    switch (supercap_data.state) {
        case SUPERCAP_STATE_INIT:
            supercap_data.state = SUPERCAP_STATE_IDLE;
            break;
            
        case SUPERCAP_STATE_IDLE:
            // Check if we should start charging
            if (supercap_data.supercap_voltage < SUPERCAP_RATED_VOLTAGE &&
                supercap_data.battery_voltage > BATTERY_MIN_VOLTAGE) {
                supercap_data.state = SUPERCAP_STATE_CHARGING;
            }
            // Check for back-EMF recovery opportunity
            else if (supercap_data.back_emf_recovery_enable &&
                     fabsf(supercap_data.back_emf_current) > BACK_EMF_THRESHOLD) {
                supercap_data.state = SUPERCAP_STATE_BACK_EMF_RECOVERY;
            }
            break;
            
        case SUPERCAP_STATE_CHARGING:
            Supercap_ChargingControl();
            
            // Switch to discharging if power is needed
            if (supercap_data.output_power > supercap_data.power_limit * 0.8f &&
                supercap_data.supercap_voltage > SUPERCAP_MIN_VOLTAGE) {
                supercap_data.state = SUPERCAP_STATE_DISCHARGING;
            }
            // Return to idle if fully charged
            else if (supercap_data.supercap_voltage >= SUPERCAP_RATED_VOLTAGE) {
                supercap_data.state = SUPERCAP_STATE_IDLE;
            }
            break;
            
        case SUPERCAP_STATE_DISCHARGING:
            Supercap_DischargingControl();
            
            // Return to charging if voltage too low
            if (supercap_data.supercap_voltage <= SUPERCAP_MIN_VOLTAGE) {
                supercap_data.state = SUPERCAP_STATE_CHARGING;
            }
            // Return to idle if power demand is low
            else if (supercap_data.output_power < supercap_data.power_limit * 0.5f) {
                supercap_data.state = SUPERCAP_STATE_IDLE;
            }
            break;
            
        case SUPERCAP_STATE_BACK_EMF_RECOVERY:
            Supercap_BackEMFRecoveryControl();
            
            // Return to idle when back-EMF is depleted
            if (fabsf(supercap_data.back_emf_current) < BACK_EMF_THRESHOLD * 0.5f) {
                supercap_data.state = SUPERCAP_STATE_IDLE;
            }
            break;
            
        case SUPERCAP_STATE_ERROR:
        case SUPERCAP_STATE_PROTECT:
            // Stay in protection state until errors are cleared
            break;
    }
}

/**
 * @brief Charging control logic
 */
static void Supercap_ChargingControl(void)
{
    float target_current = MAX_CHARGE_CURRENT;
    float voltage_diff = SUPERCAP_RATED_VOLTAGE - supercap_data.supercap_voltage;
    
    // Reduce charging current as we approach target voltage (CC-CV charging)
    if (voltage_diff < 2.0f) {
        target_current = MAX_CHARGE_CURRENT * (voltage_diff / 2.0f);
    }
    
    // Limit charging power
    float max_current_from_power = supercap_data.power_limit / supercap_data.battery_voltage;
    if (target_current > max_current_from_power) {
        target_current = max_current_from_power;
    }
    
    // Calculate duty cycle based on current error
    float current_error = target_current - supercap_data.charge_current;
    float duty_cycle = supercap_data.duty_cycle + (current_error * 0.01f); // Simple PI control
    
    // Clamp duty cycle
    if (duty_cycle > 0.95f) duty_cycle = 0.95f;
    if (duty_cycle < 0.0f) duty_cycle = 0.0f;
    
    supercap_data.duty_cycle = duty_cycle;
    supercap_data.charge_enable = true;
    supercap_data.discharge_enable = false;
    
    Supercap_UpdatePWM(duty_cycle);
}

/**
 * @brief Discharging control logic
 */
static void Supercap_DischargingControl(void)
{
    float target_power = supercap_data.power_limit;
    float max_discharge = MAX_DISCHARGE_CURRENT * supercap_data.supercap_voltage;
    
    if (target_power > max_discharge) {
        target_power = max_discharge;
    }
    
    float target_current = target_power / supercap_data.supercap_voltage;
    float current_error = target_current - supercap_data.discharge_current;
    float duty_cycle = supercap_data.duty_cycle + (current_error * 0.01f);
    
    // Clamp duty cycle
    if (duty_cycle > 0.95f) duty_cycle = 0.95f;
    if (duty_cycle < 0.0f) duty_cycle = 0.0f;
    
    supercap_data.duty_cycle = duty_cycle;
    supercap_data.charge_enable = false;
    supercap_data.discharge_enable = true;
    
    Supercap_UpdatePWM(duty_cycle);
}

/**
 * @brief Back-EMF recovery control logic (Key Feature)
 */
static void Supercap_BackEMFRecoveryControl(void)
{
    // When wheels decelerate, they generate back-EMF
    // This function captures and stores that energy in the supercapacitor
    
    float recovery_voltage = fabsf(supercap_data.back_emf_current) * 0.1f; // Simplified model
    float voltage_headroom = SUPERCAP_MAX_VOLTAGE - supercap_data.supercap_voltage;
    
    // Only recover if we have voltage headroom
    if (voltage_headroom > 0.5f) {
        float duty_cycle = 0.3f; // Conservative duty cycle for recovery
        
        // Scale duty cycle based on voltage headroom
        if (voltage_headroom < 2.0f) {
            duty_cycle *= (voltage_headroom / 2.0f);
        }
        
        supercap_data.duty_cycle = duty_cycle;
        supercap_data.charge_enable = true;
        supercap_data.discharge_enable = false;
        
        // Track recovered energy
        float recovered_power = recovery_voltage * fabsf(supercap_data.back_emf_current);
        supercap_data.energy_recovered += recovered_power / CONTROL_LOOP_FREQ_HZ;
        
        Supercap_UpdatePWM(duty_cycle);
    }
}

/**
 * @brief Calculate stored energy in supercapacitor
 */
static float Supercap_CalculateEnergy(float voltage)
{
    // E = 0.5 * C * V^2
    // Assuming equivalent capacitance of 50F for typical supercap bank
    const float CAPACITANCE = 50.0f;
    return 0.5f * CAPACITANCE * voltage * voltage;
}

/**
 * @brief Update PWM output
 */
static void Supercap_UpdatePWM(float duty_cycle)
{
    // TODO: Implement actual PWM update for hardware
    // This is a placeholder
    
    // Example: TIM1->CCR1 = (uint32_t)(duty_cycle * PWM_PERIOD);
}

/**
 * @brief Safety checks and protection
 */
void Supercap_SafetyCheck(void)
{
    uint32_t errors = ERROR_NONE;
    
    // Check overvoltage
    if (supercap_data.supercap_voltage > OVERVOLTAGE_THRESHOLD) {
        errors |= ERROR_OVERVOLTAGE;
    }
    
    // Check undervoltage
    if (supercap_data.supercap_voltage < UNDERVOLTAGE_THRESHOLD &&
        supercap_data.state != SUPERCAP_STATE_INIT) {
        errors |= ERROR_UNDERVOLTAGE;
    }
    
    // Check overcurrent
    if (supercap_data.charge_current > OVERCURRENT_THRESHOLD ||
        supercap_data.discharge_current > OVERCURRENT_THRESHOLD) {
        errors |= ERROR_OVERCURRENT;
    }
    
    // Check overtemperature
    if (supercap_data.temperature > OVERTEMP_THRESHOLD) {
        errors |= ERROR_OVERTEMP;
    }
    
    // Check battery voltage
    if (supercap_data.battery_voltage < BATTERY_MIN_VOLTAGE ||
        supercap_data.battery_voltage > BATTERY_MAX_VOLTAGE) {
        errors |= ERROR_SENSOR_FAULT;
    }
    
    supercap_data.error_flags = errors;
    
    // Enter protection state if any critical error
    if (errors != ERROR_NONE) {
        supercap_data.state = SUPERCAP_STATE_PROTECT;
    }
}

/**
 * @brief Emergency stop
 */
void Supercap_EmergencyStop(void)
{
    supercap_data.charge_enable = false;
    supercap_data.discharge_enable = false;
    supercap_data.duty_cycle = 0.0f;
    Supercap_UpdatePWM(0.0f);
}

/**
 * @brief Clear error flags
 */
void Supercap_ClearErrors(void)
{
    supercap_data.error_flags = ERROR_NONE;
    if (supercap_data.state == SUPERCAP_STATE_ERROR ||
        supercap_data.state == SUPERCAP_STATE_PROTECT) {
        supercap_data.state = SUPERCAP_STATE_IDLE;
    }
}

/**
 * @brief Set power limit
 */
void Supercap_SetPowerLimit(float power_limit)
{
    if (power_limit > 0.0f && power_limit <= 200.0f) {
        supercap_data.power_limit = power_limit;
    }
}

/**
 * @brief Enable/disable back-EMF recovery
 */
void Supercap_SetBackEMFRecovery(bool enable)
{
    supercap_data.back_emf_recovery_enable = enable;
}

/**
 * @brief Get system data
 */
const SupercapData_t* Supercap_GetData(void)
{
    return &supercap_data;
}

/**
 * @brief Get current state
 */
SupercapState_t Supercap_GetState(void)
{
    return supercap_data.state;
}

/**
 * @brief Process back-EMF and return recovered energy
 */
float Supercap_ProcessBackEMF(void)
{
    return supercap_data.energy_recovered;
}
