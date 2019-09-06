
##############################################################################
#
# BASE
#


# CC?=arm-linux-gnueabihf-gcc

ARM_CC ?= arm-linux-gnueabihf-gcc
GCC = gcc

LIBS=bcm2835

CFLAGS+=-pthread
CFLAGS+=-lwiringPi  
# Expand defines
# CFLAGS += $(addprefix -D,$(DEFINES))
# DWM_PROJ_DIR = $(shell pwd)
INC_DIR = $(DWM_PROJ_DIR)/include
DRIVER_DIR = $(DWM_PROJ_DIR)/dwm_driver
LMH_DIR = $(DRIVER_DIR)/lmh
API_DIR = $(DRIVER_DIR)/dwm_api

##############################################################################
#  TARGET choice
#  0: Raspberry-Pi
#  1: else

ifeq ($(TARGET),0)
cc = $(ARM_CC)
HAL_DIR = $(DWM_PROJ_DIR)/platform/rpi/hal
endif 

SOURCEDIRS += $(INC_DIR)
SOURCEDIRS += $(DRIVER_DIR)
SOURCEDIRS += $(HAL_DIR)
SOURCEDIRS += $(LMH_DIR)
SOURCEDIRS += $(API_DIR)


# DWM_SOURCES += main.c
INCLUDES += $(INC_DIR)/dwm1001_tlv.h
INCLUDES += $(INC_DIR)/dwm_api.h
DWM_SOURCES += $(API_DIR)/dwm_api.c

INCLUDES += $(HAL_DIR)/hal.h
DWM_SOURCES += $(HAL_DIR)/hal.c
INCLUDES += $(HAL_DIR)/hal_fprint.h
DWM_SOURCES += $(HAL_DIR)/hal_fprint.c
INCLUDES += $(HAL_DIR)/hal_interface.h
LOGFILES += log.txt

INCLUDES += $(LMH_DIR)/lmh.h
DWM_SOURCES += $(LMH_DIR)/lmh.c

##############################################################################
#  INTERFACE_NUMBER choice
#  USE_UART           0
#  USE_SPI            1
#  USE_SPI_DRDY       2
ifndef INTERFACE_NUMBER
INTERFACE_NUMBER=0
endif

DEFINES += INTERFACE_NUMBER=$(INTERFACE_NUMBER)
ifeq ($(INTERFACE_NUMBER),0)
INCLUDES += $(HAL_DIR)/hal_uart.h
DWM_SOURCES += $(HAL_DIR)/hal_uart.c
INCLUDES += $(LMH_DIR)/lmh_uartrx.h
DWM_SOURCES += $(LMH_DIR)/lmh_uartrx.c
endif 

ifeq ($(INTERFACE_NUMBER),1)
INCLUDES += $(HAL_DIR)/hal_spi.h
DWM_SOURCES += $(HAL_DIR)/hal_spi.c
INCLUDES += $(LMH_DIR)/lmh_spirx.h
DWM_SOURCES += $(LMH_DIR)/lmh_spirx.c
endif 

ifeq ($(INTERFACE_NUMBER),2)
INCLUDES += $(HAL_DIR)/hal_spi.h
DWM_SOURCES += $(HAL_DIR)/hal_spi.c
INCLUDES += $(HAL_DIR)/hal_gpio.h
DWM_SOURCES += $(HAL_DIR)/hal_gpio.c
INCLUDES += $(LMH_DIR)/lmh_spirx.h
DWM_SOURCES += $(LMH_DIR)/lmh_spirx.c
INCLUDES += $(LMH_DIR)/lmh_spirx_drdy.h
DWM_SOURCES += $(LMH_DIR)/lmh_spirx_drdy.c
endif 

DEFINES += PRINT_LVL=$(PRINT_LVL)

# Expand defines
CFLAGS += $(addprefix -D,$(DEFINES))

# Expand search paths
CFLAGS += $(addprefix -I ,$(SOURCEDIRS))

vpath %.c $(SOURCEDIRS)


$(DWM_PROGRAM): $(DWM_SOURCES) $(INCLUDES)
	$(GCC) -g -o $@ $(DWM_SOURCES) -l $(LIBS) $(CFLAGS)
	@echo $@ "build done"  
   
clean:
	@echo "Cleaning"
	$(Q)-rm -f $(DWM_PROGRAM)
	$(Q)-rm -f $(LOGFILES)
   
   
