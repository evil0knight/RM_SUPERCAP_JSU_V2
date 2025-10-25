# RM_SUPERCAP_JSU_V2 Configuration Guide

## Quick Configuration

### Hardware Configuration
Edit `Firmware/Core/Inc/supercap_control.h`:

```c
// Voltage limits - adjust for your supercapacitor bank
#define SUPERCAP_MAX_VOLTAGE        24.0f   // Maximum safe voltage
#define SUPERCAP_MIN_VOLTAGE        12.0f   // Minimum discharge voltage
#define SUPERCAP_RATED_VOLTAGE      16.0f   // Target operating voltage

// Current limits - adjust for your power stage
#define MAX_CHARGE_CURRENT          10.0f   // Maximum charging current (A)
#define MAX_DISCHARGE_CURRENT       20.0f   // Maximum discharge current (A)

// Safety thresholds
#define OVERTEMP_THRESHOLD          75.0f   // Over-temperature limit (°C)
#define OVERCURRENT_THRESHOLD       25.0f   // Over-current limit (A)
```

### Control Parameters
```c
// Back-EMF recovery threshold
#define BACK_EMF_THRESHOLD          0.5f    // Minimum current to trigger recovery (A)

// Power limit
#define POWER_LIMIT_DEFAULT         80.0f   // Default power limit (W)
```

## Calibration

### Voltage Sensing
1. Measure actual voltages with a multimeter
2. Adjust ADC calibration factors
3. Typical divider ratio: 1:10 (adjust for your circuit)

```c
// In ADC reading function:
float voltage = (adc_value * 3.3f / 4096.0f) * 10.0f;  // Adjust multiplier
```

### Current Sensing
1. Use known load resistor
2. Measure actual current with ammeter
3. Adjust calibration factors

```c
// For ACS712-20A: 100mV/A, centered at 2.5V
float current = ((adc_voltage - 2.5f) / 0.1f);
```

### PWM Configuration
- Frequency: 20kHz (adjust in timer configuration)
- Resolution: 1000 steps (0-100% duty cycle)

## Build Configuration

### For STM32CubeIDE
1. Import project
2. Select your target MCU
3. Configure peripherals:
   - ADC: 3 channels
   - TIM1: PWM output
   - CAN: 1Mbps
4. Build and flash

### For Makefile
1. Edit `Firmware/Makefile`
2. Set your MCU type
3. Add HAL library paths
4. Add linker script
5. Run `make`

## Testing Configuration

### Bench Testing (Safe Mode)
```c
// Reduce limits for initial testing
#define MAX_CHARGE_CURRENT          2.0f    // 2A for testing
#define MAX_DISCHARGE_CURRENT       5.0f    // 5A for testing
#define SUPERCAP_MAX_VOLTAGE        18.0f   // Lower max for safety
```

### Production Configuration
Restore to full values after testing confirms safe operation.

## Communication Configuration

### CAN Baud Rate
Default: 1 Mbps (RoboMaster standard)

To change:
```c
// In CAN initialization
CAN_InitTypeDef.Prescaler = 4;  // Adjust for your clock
```

### CAN IDs
Edit in `can_interface.h`:
```c
#define CAN_SUPERCAP_TX_ID          0x211
#define CAN_SUPERCAP_RX_ID          0x210
```

## Advanced Tuning

### Control Loop Gains
The system uses simple PI control. For better performance:

```c
// In supercap_control.c, adjust these constants:
float current_error = target_current - actual_current;
float duty_cycle += current_error * 0.01f;  // Change 0.01f to tune
```

### State Transition Thresholds
```c
// In Supercap_StateManager()
// Adjust these conditions for your application:
if (supercap_voltage < SUPERCAP_RATED_VOLTAGE * 0.9f)  // 90% threshold
if (output_power > power_limit * 0.8f)  // 80% threshold
```

## Troubleshooting Configuration

### System Not Charging
- Check `MAX_CHARGE_CURRENT` is not too low
- Verify voltage thresholds allow charging
- Check PWM output is enabled

### Oscillation/Instability
- Reduce control loop gains
- Increase filtering on sensor readings
- Check for ground loops

### Back-EMF Not Recovering
- Lower `BACK_EMF_THRESHOLD` 
- Verify motor connections
- Check rectifier circuit

## Safety Configuration

**Never disable safety features in production!**

For testing only, you can temporarily adjust:
```c
// WARNING: For testing only!
#define OVERTEMP_THRESHOLD          85.0f   // Higher limit
#define OVERCURRENT_THRESHOLD       30.0f   // Higher limit
```

Always restore safe values for normal operation.
