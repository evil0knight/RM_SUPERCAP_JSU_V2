# Schematic Files

## Overview
This directory contains the hardware schematic files for the RM_SUPERCAP_JSU_V2 system.

## File Organization
```
Schematics/
├── README.md                    (this file)
├── supercap_main.sch           (Main power schematic - to be created)
├── mcu_control.sch             (MCU and control circuitry - to be created)
├── sensors.sch                 (Sensor interfaces - to be created)
└── can_communication.sch       (CAN interface - to be created)
```

## Schematic Sections

### 1. Main Power Stage (supercap_main.sch)
**Components:**
- Input protection (fuse, TVS, reverse polarity)
- Buck-boost converter (bidirectional DC-DC)
- MOSFETs and gate drivers
- Power inductor and capacitors
- Supercapacitor bank connection

**Key Features:**
- Bidirectional power flow
- 30A continuous current capability
- Overload protection

### 2. MCU Control (mcu_control.sch)
**Components:**
- STM32F103C8T6 MCU
- Crystal oscillator (8MHz)
- Power supply (3.3V regulator)
- Debug connector (SWD)
- Status LEDs

**Connections:**
- PWM output to gate drivers
- ADC inputs from sensors
- CAN transceiver interface
- GPIO for enables and status

### 3. Sensor Interface (sensors.sch)
**Components:**
- Voltage dividers (battery and supercap)
- Current sensors (ACS712 or similar)
- Temperature sensor (NTC thermistor)
- Op-amp buffers and filters

**Measurements:**
- Battery voltage (0-30V → 0-3.3V)
- Supercap voltage (0-30V → 0-3.3V)
- Charge current (0-30A → 0-3.3V)
- Discharge current (0-30A → 0-3.3V)
- Back-EMF current (±20A → 0-3.3V)
- Temperature (0-100°C → 0-3.3V)

### 4. CAN Communication (can_communication.sch)
**Components:**
- CAN transceiver (TJA1050)
- Termination resistor (120Ω, switchable)
- ESD protection
- Connectors

## Design Tools

### Recommended Software
- KiCad (Open source, recommended)
- Altium Designer
- Eagle CAD
- EasyEDA

### Design Files Format
- Native format: KiCad (.kicad_sch)
- Export: PDF for documentation
- BOM: CSV format

## Design Considerations

### Power Traces
- Minimum width: 3mm (100mil)
- Use copper pour for high current
- Keep inductance low

### Signal Integrity
- Differential pairs for CAN (120Ω impedance)
- Kelvin connections for current sense
- Star ground topology

### Safety
- Isolation between power and control
- Proper clearances (2mm minimum)
- Fusing on all power inputs

## PCB Requirements

### Specifications
- Layers: 4-layer recommended (2-layer possible)
- Copper weight: 2oz (70μm) on power layers
- Board thickness: 1.6mm
- Surface finish: HASL or ENIG
- Solder mask: Green (or other color)

### Size
- Maximum: 100mm x 80mm
- Mounting: 4x M3 holes

## Testing Points

Add test points for:
- All power rails
- ADC inputs
- PWM outputs
- CAN signals
- Critical voltages

## Bill of Materials (BOM)

Generate BOM with:
- Part number
- Description
- Quantity
- Reference designators
- Manufacturer
- Supplier part number

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | TBD | Initial schematic design |
| 2.0 | TBD | Added back-EMF recovery circuit |

## Contributing

When adding/modifying schematics:
1. Follow existing naming conventions
2. Use descriptive net names
3. Add comments for complex circuits
4. Update this README
5. Generate new PDF exports

## References

- [Hardware Design Guide](../HARDWARE_DESIGN.md)
- [Configuration Guide](../../Docs/CONFIGURATION.md)
- [Detailed Documentation](../../Docs/README_DETAILED.md)

## Tools and Resources

### Component Selection
- [Octopart](https://octopart.com) - Component search
- [DigiKey](https://www.digikey.com) - Supplier
- [Mouser](https://www.mouser.com) - Supplier
- [LCSC](https://lcsc.com) - Low-cost supplier

### Design Calculators
- [TI Power Design Tools](https://www.ti.com/design-resources/design-tools-simulation.html)
- [Analog Devices Design Center](https://www.analog.com/en/design-center.html)
- [Saturn PCB Toolkit](https://saturnpcb.com/saturn-pcb-toolkit/)

---

**Note**: Actual schematic files should be created using appropriate EDA software based on the specifications provided in the documentation.
