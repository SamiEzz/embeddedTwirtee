######################################################################################################
## Decawave DWM Drivers - make variables
######################################################################################################

#  Configurations

#  TARGET ={0->Raspberry-Pi ,  1->else}
TARGET = 0

#  INTERFACE_NUMBER={0: USE_UART,1: USE_SPI, 2: USE_SPI_DRDY}
INTERFACE_NUMBER = 0

#  PRINT_LVL 
#  for   HAL_Print         
#  0:    none      
#  1:    fprintf (log.txt)       
#  2:    printf (terminal)  
#  3:    printf + fprintf    
PRINT_LVL = 0

# PROJ_DIR += ./DWM_api/loc_api
DWM_PROJ_DIR 	= ./io_com_service/include/DWM_api/loc_api

# DWM_SOURCES
DWM_SOURCES 	= $(PROJ_DIR)/src/loc_thread.c

# DWM binary program
DWM_PROGRAM		= $(PROJ_DIR)/../../../build/dwm_binary.o


# DWM FLAGS
# CFLAGS += -Wall 

# include dwm1001.mak :  include $(PROJ_DIR)/include/dwm1001.mak
#include $(PROJ_DIR)/include/dwm1001.mak

######################################################################################################
## 
##########################################################################################################


LIBS= -lpthread -lm -lc
CC=gcc

RM=/bin/rm

PROGRAM = ./bin/twirtee.bin
SOURCES =./src/kanayama.c
SOURCES +=./src/guidance_mgr.c
SOURCES +=./src/measurement_randomization.c
SOURCES +=./src/segment.c
SOURCES +=./src/simulator.c
SOURCES +=./src/parametres.c
SOURCES +=./src/mat.c
SOURCES +=./src/threads_mgr.c
SOURCES +=./src/spf_thread.c
SOURCES +=./src/dijkstra.c
SOURCES +=./src/jsonApi.c
SOURCES +=./src/misc.c
SOURCES +=./src/localization.c
SOURCES +=./src/kalman.c
SOURCES +=./src/speed_selection.c


SOURCES += main.c 

#SOURCES +=./include/linux-can-utils/lib.c
#SOURCES +=./include/linux-can-utils/read_can.c






OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

OBJECTS+=./build/read_can.o
OBJECTS+=./build/write_can.o
OBJECTS+=./build/lib_can.o
OBJECTS+=./build/io_com_service.o



#./build/dwm_api.o
$(PROGRAM): $(OBJECTS) 
	$(CC) -o  $@ $(OBJECTS)  $(LIBS)
	@echo $@ "build done"
	mv ./src/*.o ./build/
	mv *.o ./build/

%.o: %.c can_thread io_service
	$(CC) $(CFLAGS) -g -c -o $@ $< 

can_thread :  
	gcc -g -c ./include/io_com_service/linux-can-utils/lib.c -o ./build/lib_can.o 
	gcc -g -c ./include/io_com_service/linux-can-utils/read_can.c ./build/lib_can.o  -o ./build/read_can.o  -w
	gcc -g -c ./include/io_com_service/linux-can-utils/write_can.c ./build/lib_can.o  -o ./build/write_can.o  -w

io_service :
	gcc -g -c ./include/io_com_service/io_com_service.c -o ./build/io_com_service.o -w

clean:
	@echo "Cleaning"
	$(RM) -f $(PROGRAM)
	$(RM) ./build/*.o
	$(RM) ./src/*.o
	
