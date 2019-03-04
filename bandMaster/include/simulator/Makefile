#all:
#	gcc -g main.c simulator.c measurement_randomization.c -pthread -lm


# $(PROGRAM): $(SOURCES) $(INCLUDES)
# 	$(cc) -g -o $@ $(SOURCES) $(LIBS) 
# 	-L(PATH)
# 	-lm (libm.a)
# 	@echo $@ "build done"

#cc = arm-linux-gnueabihf-gcc
cc=/usr/bin/arm-linux-gnueabihf-gcc
ld=/usr/bin/arm-linux-gnueabihf-ld

CFLAGS = 
PATH = /usr/lib/arm-linux-gnueabihf/
LIBS = -lpthread -lm -lc

RM=/bin/rm

PROGRAM = mysimu

SOURCES = main.c 
SOURCES += kanayama.c
SOURCES += guidance_mgr.c
SOURCES += measurement_randomization.c
SOURCES += segment.c
SOURCES += simulator.c
SOURCES += parametres.c
SOURCES += mat.c
SOURCES += threads_mgr.c

OBJECTS=$(patsubst %c, %o, $(SOURCES))


%.o: %.c
	$(cc) $(CFLAGS) -c -o $@ $<

$(PROGRAM): $(OBJECTS)
	$(ld) -o $@ $(OBJECTS) -Map $@.map -L$(PATH) $(LIBS) -e main $<
	@echo $@ "build done"

clean:
	@echo "Cleaning"
	$(RM) -f $(PROGRAM)
	$(RM) -f $(LOGFILES)
   