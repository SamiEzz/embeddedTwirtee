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
PROJ_DIR 	= ./DWM_api/loc_api

# DWM_SOURCES
DWM_SOURCES = $(PROJ_DIR)/src/loc_thread.c


# DWM FLAGS

CFLAGS += -Wall 

######################################################################################################
## 
##########################################################################################################


LIBS= -lpthread -lm -lc
CC=gcc

RM=/bin/rm

PROGRAM = ./bin/mysimu

SOURCES = main.c 
SOURCES +=./src/kanayama.c
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



OBJECTS=$(patsubst %.c, %.o, $(SOURCES))



$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)
	@echo $@ "build done"
	mv ./src/*.o ./build/
	mv *.o ./build/

%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	@echo "Cleaning"
	$(RM) -f $(PROGRAM)
	$(RM) ./src/*.o
	$(RM) ./build/*.o
	
