# THIS MAKEFILE IS TO JUST RUN THE STM32 EXAMPLES
TARGET = adc

# Must match a name in Drivers/BSP/
BOARD_IDENTIFIER = STM32469I-Discovery

# Includes just the .c file. Can't use wildcard cause of QSPI thing
BOARD_INCLUDE_SRC := stm32469i_discovery

SRC_DIR = Src
INC_DIR = Inc

# User code
USR_MODULES_DIR = $(SRC_DIR)/modules
USR_MODULES_DIR_PDM = $(USR_MODULES_DIR)/pdm

# Drivers
DRIV_DIR = /home/paulo/Alexandria/STM32Cube_FW_F4_V1.23.0/Drivers
DRIV_HAL_DIR = $(DRIV_DIR)/STM32F4xx_HAL_Driver
DRIV_HAL_DIR_INC = $(DRIV_HAL_DIR)/Inc
DRIV_HAL_DIR_SRC = $(DRIV_HAL_DIR)/Src

# CMSIS
DRIV_CMSIS_DIR = $(DRIV_DIR)/CMSIS
DRIV_CMSIS_INC = $(DRIV_CMSIS_DIR)/Include
DRIV_CMSIS_DEVICE_INC = $(DRIV_CMSIS_DIR)/Device/ST/STM32F4xx/Include
DRIV_CMSIS_DEVICE_SRC = $(DRIV_CMSIS_DIR)/Device/ST/STM32F4xx/Source

# Board support package
BSP_DIR = $(DRIV_DIR)/BSP/$(BOARD_IDENTIFIER)

DEBUG = 1
OPT = -Og

# BSP stuff... this is example specific
C_SOURCES := $(BSP_DIR)/stm32469i_discovery_sdram.c
C_SOURCES += $(BSP_DIR)/stm32469i_discovery_lcd.c
C_SOURCES += $(DRIV_DIR)/BSP/Components/otm8009a/otm8009a.c

C_SOURCES += $(wildcard $(SRC_DIR)/*.c)
C_SOURCES += $(wildcard $(DRIV_HAL_DIR_SRC)/*.c)
C_SOURCES += $(wildcard $(USR_MODULES_DIR)/*.c)
C_SOURCES += $(wildcard $(BSP_DIR)/$(BOARD_INCLUDE_SRC).c)

# Filter out templates in drivers
C_SOURCES := $(filter-out $(wildcard $(DRIV_HAL_DIR_SRC)/*template.c), $(C_SOURCES))

# Build path
BUILD_DIR = build

# ASM sources
ASM_SOURCES = SW4STM32/startup_stm32f469xx.s

#######################################
# binaries
#######################################
PREFIX = /usr/local/bin/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F469xx


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES = -I$(DRIV_HAL_DIR_INC)
C_INCLUDES += -I$(INC_DIR)
C_INCLUDES += -I$(DRIV_CMSIS_DEVICE_INC)
C_INCLUDES += -I$(DRIV_CMSIS_INC)
C_INCLUDES += -I$(USR_MODULES_DIR)
C_INCLUDES += -I$(USR_MODULES_DIR_PDM)/Inc
C_INCLUDES += -I$(BSP_DIR)
C_INCLUDES += -I$(DRIV_DIR)/BSP/Components/otm8009a

# print:
# 	echo $(C_INCLUDES)

# -IInc \
# -IDrivers/STM32F4xx_HAL_Driver/Inc \
# -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
# -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
# -IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = SW4STM32/LCD_DSI_VideoMode_DoubleBuffering/STM32F469NIHx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections\
				 -lc -lrdimon -u _printf_float

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the appliexport LD_LIBRARY_PATH=/usr/local/libcation
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
# Precompiled libraries
OBJECTS += $(wildcard $(USR_MODULES_DIR_PDM)/Lib/*.a)

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
# Flashing
#######################################
flash: all
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

