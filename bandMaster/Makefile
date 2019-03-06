PATH=/usr/include/arm-linux-gnueabihf/
LIBS= -lpthread -lm -lc
CC=/usr/bin/arm-linux-gnueabihf-gcc
AS=/usr/bin/arm-linux-gnueabihf-as

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
	$(RM) ./build/*.o
	$(RM) ./src/*.o
	