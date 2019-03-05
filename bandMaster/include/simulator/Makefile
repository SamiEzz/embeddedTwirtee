PATH=/usr/include/arm-linux-gnueabihf/
LIBS= -lpthread -lm -lc
CC=arm-linux-gnueabihf-gcc
RM=/bin/rm

PROGRAM = mysimu

SOURCES = main.c 
SOURCES +=./src/kanayama.c
SOURCES +=./src/guidance_mgr.c
SOURCES +=./src/measurement_randomization.c
SOURCES +=./src/segment.c
SOURCES +=./src/simulator.c
SOURCES +=./src/parametres.c
SOURCES +=./src/mat.c
SOURCES +=./src/threads_mgr.c


OBJECTS=$(patsubst %.c, %.o, $(SOURCES))


%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<


$(PROGRAM): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)
	$(RM) *.o
	@echo $@ "build done"

clean:
	@echo "Cleaning"
	$(RM) -f $(PROGRAM) $(OBJECTS)