# Hardware Design Overview

## Block Diagram

```
                    RM_SUPERCAP_JSU_V2 System Architecture
                    
Battery Input (6S LiPo)                                    Chassis Output
    22.2V Nominal                                             Variable
         │                                                        ▲
         │                                                        │
         ▼                                                        │
    ┌────────────────────────────────────────────────────────────┴───┐
    │                                                                 │
    │   ┌──────────────┐                                             │
    │   │   Input      │                                             │
    │   │  Protection  │   Fuse, TVS, Reverse Polarity               │
    │   │   Circuit    │                                             │
    │   └──────┬───────┘                                             │
    │          │                                                      │
    │          ▼                                                      │
    │   ┌──────────────────────────────────────────────┐            │
    │   │        Buck-Boost Converter                  │            │
    │   │  (Bidirectional DC-DC for Charge/Discharge) │            │
    │   │                                              │            │
    │   │  Primary MOSFETs: 4x N-Channel              │            │
    │   │  Driver: High-side/Low-side gate driver     │            │
    │   │  Inductor: 47uH, 30A rated                  │            │
    │   │  Switching Freq: 20kHz                      │            │
    │   └──────┬───────────────────────────────────────┘            │
    │          │                                                      │
    │          ▼                                                      │
    │   ┌──────────────────────────┐                                │
    │   │   Supercapacitor Bank    │                                │
    │   │                          │                                │
    │   │   Configuration: 2S2P    │   (Example configuration)      │
    │   │   Individual cap: 350F   │                                │
    │   │   Total: ~50F @ 16V      │                                │
    │   │   ESR: < 5mΩ             │                                │
    │   └──────┬───────────────────┘                                │
    │          │                                                      │
    │          └──────────────────────────────────────────────┐     │
    │                                                          │     │
    │   ┌───────────────────────────────────────────────┐     │     │
    │   │       Back-EMF Recovery Circuit               │     │     │
    │   │                                               │     │     │
    │   │  - Rectifier bridge for AC motors           │     │     │
    │   │  - Current sensing (bidirectional)          │     │     │
    │   │  - Synchronous rectification                 │     │     │
    │   │  - Connects to motor phase lines             │     │     │
    │   └────────┬──────────────────────────────────────┘     │     │
    │            │                                             │     │
    │            └─────────────────────────────────────────────┘     │
    │                                                                 │
    │   ┌─────────────────────────────────────────────────────┐     │
    │   │          Control & Monitoring MCU                   │     │
    │   │                                                      │     │
    │   │  MCU: STM32F103C8T6 or equivalent                   │     │
    │   │  Core: ARM Cortex-M3, 72MHz                         │     │
    │   │  Flash: 64KB, RAM: 20KB                             │     │
    │   │                                                      │     │
    │   │  Peripherals:                                        │     │
    │   │  - ADC: 3 channels (voltage, current, temp)        │     │
    │   │  - PWM: Timer1 for buck-boost control               │     │
    │   │  - CAN: CAN1 for RoboMaster communication          │     │
    │   │  - GPIO: Status LEDs, enable signals                │     │
    │   │                                                      │     │
    │   │  Sensors:                                            │     │
    │   │  - Voltage dividers (1:10 ratio)                   │     │
    │   │  - Current sensors: ACS712 or hall-effect          │     │
    │   │  - Temperature: NTC thermistor 10K                  │     │
    │   └──────────────────────────────────────────────────────┘     │
    │                                                                 │
    └─────────────────────────────────────────────────────────────────┘

External Connections:
- CAN_H/CAN_L: To chassis controller
- Motor Phase: To motor controller for back-EMF sensing
- Status LEDs: Power, Charge, Discharge, Error
```

## Key Components

### Power Stage
1. **MOSFETs**: 
   - Type: N-Channel, 60V, 100A
   - Qty: 4 (H-bridge configuration)
   - Part example: IRFB4115PBF or equivalent

2. **Gate Drivers**:
   - High-side and low-side driver
   - Part example: IR2110, UCC27211

3. **Inductor**:
   - Value: 47μH
   - Current rating: 30A
   - DCR: < 5mΩ

4. **Capacitors**:
   - Input: 100μF ceramic + 470μF electrolytic, 35V
   - Output: Similar configuration
   - Gate drive: 1μF ceramic per driver

### Control Stage
1. **MCU**: STM32F103C8T6 (Blue Pill) or similar
   - Alternative: STM32F303, STM32F405

2. **CAN Transceiver**: TJA1050 or SN65HVD230

3. **Current Sensors**:
   - ACS712-20A (bidirectional)
   - Or: INA219 (I2C current/voltage monitor)

4. **Voltage Sensing**:
   - Resistor dividers (1:10 ratio)
   - Op-amp buffers for isolation

### Protection Components
1. **Input Protection**:
   - Fuse: 30A fast-blow
   - TVS diode: SMAJ26A (26V clamping)
   - Reverse polarity: P-channel MOSFET

2. **Supercap Protection**:
   - Balancing circuit for series cells
   - Voltage monitor IC: LTC3350 or similar

## PCB Design Considerations

### Layout Guidelines
1. **Power Traces**:
   - Minimum width: 3mm (100mil) for 30A
   - Use copper pour on both layers
   - Keep high-current paths short

2. **Signal Integrity**:
   - Separate analog and digital grounds
   - Star ground at power supply
   - CAN traces: differential pair, 120Ω impedance

3. **Thermal Management**:
   - Large copper areas under MOSFETs
   - Thermal vias to bottom layer
   - Heatsink provision for high-power MOSFETs

4. **Layer Stack** (4-layer recommended):
   - Layer 1: Top - signal and small power
   - Layer 2: Ground plane
   - Layer 3: Power plane
   - Layer 4: Bottom - signal and large power

### Critical Dimensions
- Board size: ~100mm x 80mm
- Mounting holes: M3, 4 corners
- Connector spacing: Allow for XT60/XT90 connectors
- Clearance: 2mm minimum between high voltage traces

## Supercapacitor Selection

### Recommended Specifications
- **Type**: Maxwell/Nesscap/LS Mtron EDLC
- **Individual Cell**: 350F - 400F, 2.7V
- **Configuration**: 2S2P (2 series, 2 parallel)
  - Resulting: ~50F @ 16V
- **ESR**: < 5mΩ per cell
- **Lifetime**: 500,000+ cycles

### Balancing
- Use passive balancing resistors (10Ω, 2W per cell)
- Or active balancing IC for better efficiency

## Bill of Materials (Partial)

| Item | Part Number | Qty | Notes |
|------|-------------|-----|-------|
| MCU | STM32F103C8T6 | 1 | Or equivalent |
| CAN Transceiver | TJA1050 | 1 | 3.3V/5V compatible |
| MOSFET | IRFB4115PBF | 4 | Or equivalent 60V/100A |
| Gate Driver | IR2110 | 2 | High-side/low-side |
| Inductor | 47μH/30A | 1 | Powdered iron core |
| Current Sensor | ACS712-20A | 3 | Charge, discharge, back-EMF |
| Supercap | 350F/2.7V | 4 | Series-parallel config |
| TVS Diode | SMAJ26A | 1 | Input protection |
| Fuse | 30A Fast | 1 | Input protection |

## Testing Points

### Initial Testing
1. **No-load test**: Verify PWM generation
2. **Low-voltage test**: Test with 12V supply, no supercap
3. **Charging test**: Monitor charging current and voltage
4. **Discharging test**: Load test with resistive load
5. **Back-EMF test**: Simulate motor deceleration

### Safety Checklist
- [ ] All protection circuits verified
- [ ] Voltage sensing calibrated
- [ ] Current sensing calibrated
- [ ] Temperature monitoring working
- [ ] CAN communication verified
- [ ] Emergency stop functional
- [ ] Thermal performance acceptable

## Firmware Configuration

See `Firmware/Core/Inc/supercap_control.h` for configuration parameters.

### Key Parameters to Adjust
```c
#define SUPERCAP_MAX_VOLTAGE        24.0f   // Adjust for your caps
#define SUPERCAP_MIN_VOLTAGE        12.0f   // Safe discharge limit
#define MAX_CHARGE_CURRENT          10.0f   // Hardware current limit
#define MAX_DISCHARGE_CURRENT       20.0f   // Hardware current limit
```

## References

1. [Maxwell Supercapacitor Datasheet](https://www.maxwell.com)
2. [Buck-Boost Converter Design](https://www.ti.com/lit/an/slva535b/slva535b.pdf)
3. [RoboMaster Power Management Rules](https://www.robomaster.com)
4. [Regenerative Braking in Electric Vehicles](https://ieeexplore.ieee.org)

---

**Note**: This is a reference design. Always verify calculations and test thoroughly before full-power operation.
