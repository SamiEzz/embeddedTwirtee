####################################################
# @file    Makefile
#
# @attention
#
# Copyright 2017 (c) Decawave Ltd, Dublin, Ireland.
#
# All rights reserved.
#

####################################################
#  Configurations

####################################################
#  TARGET
#  0: Raspberry-Pi
#  1: else
TARGET = 0

####################################################
#  INTERFACE_NUMBER
#  0: USE_UART  
#  1: USE_SPI    
#  2: USE_SPI_DRDY
INTERFACE_NUMBER = 0

####################################################
#  PRINT_LVL 
#  for   HAL_Print         
#  0:    none      
#  1:    fprintf (log.txt)       
#  2:    printf (terminal)  
#  3:    printf + fprintf    
PRINT_LVL = 0


DWM_PROGRAM = $(DWM_PROJ_DIR)/../../../../build/dwm_api.o
DWM_SOURCES =   ./src/main.c 
DWM_SOURCES +=  ./src/loc_thread.c
DWM_PROJ_DIR += ./loc_api


CFLAGS += -Wall 



include $(DWM_PROJ_DIR)/include/dwm1001.mak
