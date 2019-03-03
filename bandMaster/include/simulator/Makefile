#all:
#	gcc -g main.c simulator.c measurement_randomization.c -pthread -lm



cc =gcc

CFLAGS = -lm 
CFLAGS += -pthread

LIBS = 

PROGRAM = ./bin/simu
SOURCES = main.c 
SOURCES += kanayama.c
SOURCES += guidance_mgr.c
SOURCES += measurement_randomization.c
SOURCES += kanayama.c


INCLUDES = simulator.h
INCLUDES += kanayama.h
INCLUDES += guidance_mgr.h
INCLUDES += measurement_randomization.h
INCLUDES += ../twirtee.h

$(PROGRAM): $(SOURCES) $(INCLUDES)
	$(cc) -g -o $@ $(SOURCES) -l $(LIBS) $(CFLAGS)
	@echo $@ "build done"