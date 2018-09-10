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


######################################################
# PROJ_DIR stands for the directory of the dwm api
#

PROJ_DIR += ./include/loc_api

CFLAGS += -lm -std=c99

######################################################
# SOURCES
#

PROGRAM = ./bin/bandMaster
SOURCES = ./src/main.c 
SOURCES += ./src/loc_thread.c
SOURCES += ./src/spf_thread.c
SOURCES += ./src/jsonApi.c 
SOURCES += ./src/dijkstra.c 
SOURCES += ./src/threads_mgr.c


include $(PROJ_DIR)/include/dwm1001.mak
