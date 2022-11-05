##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.14.1] date: [Wed Oct 20 23:03:17 JST 2021] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = MeasureBoard_fw


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O3


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(wildcard Drivers/STM32F3xx_HAL_Driver/Src/*.c) \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
Src/syscalls.c \
Src/cui_encoder/AMT23.c \
BSP/MeasurementBoard_v1.c \
Src/main.c \
Src/gpio.c \
Src/can.c \
Src/dma.c \
Src/i2c.c \
Src/spi.c \
Src/tim.c \
Src/usart.c \
Src/usb_device.c \
Src/usbd_conf.c \
Src/usbd_desc.c \
Src/usbd_cdc_if.c \
Src/stm32f3xx_it.c \
Src/stm32f3xx_hal_msp.c \
Src/system_stm32f3xx.c \
Src/freertos.c \
Src/stm32f3xx_hal_timebase_tim.c \
$(wildcard Middlewares/Third_Party/FreeRTOS/Source/*.c ) \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
Src/FreeRTOS-openocd.c \
Src/rs485.c \
Middlewares/Third_Party/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI/FreeRTOS_CLI.c \
$(wildcard Src/CLI/*.c ) \
Src/scramble_tasks.c \
Src/UARTCommandConsole.c \
FreeRTOS_helpers/port_DRN.c \
FreeRTOS_helpers/heap_useNewlib_ST.c \
xprintf/xprintf.c \
src/mps_encoder/MA7xx.c \
src/IncEncoder.c \
src/bms/bq78350.c \
src/usb_cdc.c \
src/uart_util_hal.c
# ASM sources
ASM_SOURCES =  \
startup_stm32f303xc.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
DP = $(GCC_PATH)/$(PREFIX)objdump
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
DP = $(PREFIX)objdump
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
LST = $(DP) -D -Sd

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
-DSTM32F303xC

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IBSP \
-IInc \
-IDrivers/STM32F3xx_HAL_Driver/Inc \
-IDrivers/STM32F3xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/ST/STM32_USB_Device_Library/Core/Inc \
-IMiddlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc \
-IDrivers/CMSIS/Device/ST/STM32F3xx/Include \
-IDrivers/CMSIS/Include \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-IMiddlewares/Third_Party/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI \
-IMiddlewares/ST/ARM/DSP/Inc \
-Ixprintf \
-IInc/bms \
-IInc/mps_encoder \
-IInc/eeprom \
-IInc/cui_encoder \
-IInc/CLI

# float option
# -Wfloat-conversion			doubleからfloatへ暗黙の型変換が発生したときに警告
# -Wunsuffixed-float-constants	浮動小数点の定数に型を示すサフィックスがないことを警告
# -Wdouble-promotion			内部演算でflaotからdoubleへ暗黙の型変換が発生したときに警告
# -fsingle-precision-constant	浮動小数点の計算は全て単精度（float）で実施

FLOAT-OPT += -Wfloat-conversion
FLOAT-OPT += -Wunsuffixed-float-constants
FLOAT-OPT += -Wdouble-promotion
#FLOAT-OPT += -fsingle-precision-constant

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections $(FLOAT-OPT)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -std=gnu11 $(FLOAT-OPT)

CCACHE := $(shell which ccache)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F303CCTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--undefined=uxTopUsedPriority,-print-memory-usage

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).lst


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CCACHE) $(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CCACHE) $(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	

$(BUILD_DIR)/%.lst: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(LST) $< > $@

$(BUILD_DIR):
	mkdir $@		

#######################################
# Program OpenOCD
#######################################
upload_open: build/$(TARGET).bin
	openocd -f board/st_nucleo_f3.cfg -c "reset_config trst_only combined" -c "program build/$(TARGET).elf verify reset exit"

#######################################
# Program J-Link
#######################################
upload_jlink: build/$(TARGET).bin
	JLink -device STM32F303CC -if SWD -speed 4000 -autoconnect 1 -CommanderScript download_flash.jlink

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
