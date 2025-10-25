# Makefile for RM_SUPERCAP_JSU_V2 Firmware
# Target: STM32F103C8T6 or compatible

######################################
# Target
######################################
TARGET = rm_supercap_v2

######################################
# Building variables
######################################
# Debug build?
DEBUG = 1
# Optimization
OPT = -Og

#######################################
# Paths
#######################################
# Build path
BUILD_DIR = build

######################################
# Source files
######################################
# C sources
C_SOURCES = \
Firmware/Core/Src/main.c \
Firmware/Core/Src/supercap_control.c \
Firmware/Core/Src/can_interface.c

# ASM sources (add your startup file here)
# ASM_SOURCES = startup_stm32f103xb.s

#######################################
# Binaries
#######################################
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# CPU
CPU = -mcpu=cortex-m3

# FPU (none for STM32F103)
FPU = 

# Float ABI
FLOAT-ABI = 

# MCU
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# AS defines (add your chip-specific defines)
AS_DEFS = 

# C defines
C_DEFS = \
-DSTM32F103xB \
-DUSE_HAL_DRIVER

# AS includes (add your paths)
AS_INCLUDES = 

# C includes
C_INCLUDES = \
-IFirmware/Core/Inc

# Compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# Link script (add your chip-specific linker script)
# LDSCRIPT = STM32F103C8Tx_FLASH.ld

# Libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# Default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# Build the application
#######################################
# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# List of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# Clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# Dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# Help
help:
	@echo "Usage:"
	@echo "  make [all]     - Build the firmware"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make help      - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  DEBUG=$(DEBUG) (0=release, 1=debug)"
	@echo "  OPT=$(OPT)"
	@echo ""
	@echo "Note: You need to add your specific:"
	@echo "  - Startup file (ASM_SOURCES)"
	@echo "  - Linker script (LDSCRIPT)"
	@echo "  - HAL library sources"
	@echo "  - Chip-specific defines"

.PHONY: all clean help
