# PCB Design Files

## Overview
This directory will contain the printed circuit board (PCB) design files for the RM_SUPERCAP_JSU_V2 system.

## Status
🚧 **Under Development** - PCB design files are in progress

## Planned Files
```
PCB/
├── README.md                    (this file)
├── supercap_v2.kicad_pcb       (Main PCB design - to be created)
├── supercap_v2.kicad_pro       (Project file - to be created)
├── gerber/                      (Manufacturing files - to be generated)
│   ├── *.gbr                   (Gerber files)
│   ├── *.drl                   (Drill files)
│   └── *.pdf                   (Assembly drawings)
├── 3d/                          (3D models - to be created)
│   └── *.step                  (3D STEP files)
└── manufacturing/               (Manufacturing documentation)
    ├── BOM.csv                 (Bill of materials)
    └── CPL.csv                 (Component placement list)
```

## PCB Specifications

### Design Requirements
- **Size**: 100mm x 80mm (maximum)
- **Layers**: 4 layers (recommended)
  - Layer 1: Top signal and small power
  - Layer 2: Ground plane
  - Layer 3: Power plane (+24V, +16V, +3.3V)
  - Layer 4: Bottom signal and large power
- **Copper Weight**:
  - Layers 1 & 4: 2oz (70μm) for power traces
  - Layers 2 & 3: 2oz (70μm) for planes
- **Thickness**: 1.6mm
- **Surface Finish**: HASL or ENIG
- **Solder Mask**: Green (standard)
- **Silkscreen**: White

### Design Constraints
- **Minimum Trace Width**: 0.2mm (signal), 3mm (power)
- **Minimum Clearance**: 0.2mm (signal), 2mm (high voltage)
- **Via Size**: 0.3mm drill, 0.6mm pad
- **Power Via**: 0.5mm drill, 1mm pad

### Key Features
1. **Power Stage Area**
   - Large copper pours for high current
   - Thermal vias under MOSFETs
   - Short traces between components
   - Kelvin connections for current sensing

2. **Control Section**
   - Separate analog and digital grounds
   - Star ground topology
   - Bypass capacitors close to IC pins
   - Shielded sensitive traces

3. **Communication**
   - CAN bus differential pair routing
   - 120Ω controlled impedance
   - ESD protection near connectors

4. **Thermal Management**
   - Large copper areas for heat dissipation
   - Mounting provisions for heatsinks
   - Thermal vias (0.3mm, multiple)
   - Temperature sensor near MOSFETs

## Component Placement Guidelines

### Top Layer (Component Side)
```
┌─────────────────────────────────────────────┐
│  [LED]  [LED]  [LED]                  [CAN] │
│                                       [CONN] │
│  ┌──────────┐         ┌─────────────┐      │
│  │   MCU    │         │   MOSFET    │      │
│  │STM32F103 │         │  H-Bridge   │      │
│  └──────────┘         └─────────────┘      │
│                                              │
│  [Current]            [Gate]                │
│  [Sensors]            [Drivers]             │
│                                              │
│  ┌──────────┐                               │
│  │Regulator │         [Inductor]            │
│  │  3.3V    │                               │
│  └──────────┘                               │
│                                              │
│  [IN-]  [IN+]         [OUT-]  [OUT+]       │
└─────────────────────────────────────────────┘
```

### Connector Locations
- **Power Input**: Left side (XT60 or XT90)
- **Supercap Connection**: Center (high current terminals)
- **Power Output**: Right side (XT60 or XT90)
- **CAN Bus**: Top right (4-pin connector)
- **Debug**: Top center (SWD 4-pin)
- **Status LEDs**: Top left

## Assembly Notes

### Assembly Order
1. **SMD Components First**
   - Smallest to largest
   - MCU and support components
   - Resistors and capacitors
   - Current sensors

2. **Through-Hole Components**
   - Connectors
   - Large capacitors
   - Inductor
   - Terminal blocks

3. **MOSFETs Last**
   - Apply thermal paste
   - Mount heatsinks
   - Secure with screws

### Special Instructions
- **Hand Solder**: MOSFETs, large capacitors, connectors
- **Reflow**: SMD components (profile: lead-free)
- **Thermal Paste**: Between MOSFETs and heatsink
- **Torque**: M3 screws to 0.5 Nm

## Testing Points
Add test points for:
- VCC_3V3
- VCC_12V
- VCC_24V
- GND
- PWM_OUT
- ADC_VBAT
- ADC_VCAP
- ADC_ICHARGE
- ADC_IDISCHARGE
- CAN_H
- CAN_L

## Design Files Format

### KiCad Files (Recommended)
- `.kicad_pcb` - PCB layout
- `.kicad_pro` - Project file
- `.kicad_sch` - Schematic (from ../Schematics/)

### Gerber Files (for Manufacturing)
```
Gerber/
├── supercap_v2-F_Cu.gbr        (Top copper)
├── supercap_v2-B_Cu.gbr        (Bottom copper)
├── supercap_v2-In1_Cu.gbr      (Inner layer 1)
├── supercap_v2-In2_Cu.gbr      (Inner layer 2)
├── supercap_v2-F_Mask.gbr      (Top solder mask)
├── supercap_v2-B_Mask.gbr      (Bottom solder mask)
├── supercap_v2-F_Silkscreen.gbr (Top silkscreen)
├── supercap_v2-B_Silkscreen.gbr (Bottom silkscreen)
├── supercap_v2-Edge_Cuts.gbr   (Board outline)
├── supercap_v2-F_Paste.gbr     (Top paste)
├── supercap_v2-B_Paste.gbr     (Bottom paste)
└── supercap_v2.drl             (Drill file)
```

## Manufacturing

### Recommended Manufacturers
- **Prototype**: JLCPCB, PCBWay, Seeed Studio
- **Production**: Depends on volume

### Cost Estimate (Prototype)
- PCB only (5 pcs): $50-100 USD
- Assembly: $200-500 USD (depends on components)
- Total: ~$250-600 USD for first 5 units

### Lead Time
- PCB fabrication: 5-7 days
- Component sourcing: 1-2 weeks
- Assembly: 3-5 days
- **Total**: ~3-4 weeks

## Design Review Checklist

Before manufacturing:
- [ ] DRC (Design Rule Check) passed
- [ ] ERC (Electrical Rule Check) passed
- [ ] All components have footprints
- [ ] Footprints verified against datasheets
- [ ] Polarity marked on PCB
- [ ] Pin 1 indicators clear
- [ ] Mounting holes correct size
- [ ] Trace widths adequate for current
- [ ] Thermal relief on ground pours
- [ ] Silkscreen readable and not on pads
- [ ] Board outline correct
- [ ] Tooling holes added (if needed)

## Version Control

### Design Versions
- v1.0: Initial prototype (TBD)
- v1.1: First revision (TBD)
- v2.0: Production candidate (TBD)

### Change Log
Track changes in `CHANGELOG.md` in root directory

## Tools and Resources

### Design Software
- [KiCad](https://www.kicad.org/) - Free, open source
- [Altium Designer](https://www.altium.com/) - Professional
- [EAGLE](https://www.autodesk.com/products/eagle/) - Autodesk

### PCB Calculators
- [Saturn PCB Toolkit](https://saturnpcb.com/saturn-pcb-toolkit/) - Trace width, impedance
- [DigiKey PCB Calculator](https://www.digikey.com/en/resources/online-conversion-calculators)
- [EEWeb Tools](https://www.eeweb.com/tools/)

### Component Libraries
- [SnapEDA](https://www.snapeda.com/) - Free footprints
- [Component Search Engine](https://componentsearchengine.com/)
- [Ultra Librarian](https://www.ultralibrarian.com/)

### Gerber Viewers
- [Gerbv](http://gerbv.geda-project.org/) - Free viewer
- [ViewMate](https://www.pentalogix.com/viewmate/) - Free viewer
- [EasyEDA Online Viewer](https://easyeda.com/page/gerber-viewer)

## Contributing

When contributing PCB designs:
1. Use consistent naming conventions
2. Follow layer stack and design rules
3. Document all changes
4. Generate gerber files
5. Update this README
6. Create assembly drawings

## Support

For PCB design questions:
- Open an issue on GitHub
- Check KiCad forums
- Consult hardware design guide

---

**Status**: 🚧 In Development  
**Target Completion**: TBD  
**Maintainer**: Development Team

For the reference schematic and block diagram, see:
- [Hardware Design Guide](../HARDWARE_DESIGN.md)
- [Schematic Files](../Schematics/)
