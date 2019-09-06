/usr/bin/arm-linux-gnueabihf-gcc  -c -o main.o main.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o kanayama.o kanayama.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o guidance_mgr.o guidance_mgr.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o measurement_randomization.o measurement_randomization.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o segment.o segment.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o simulator.o simulator.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o parametres.o parametres.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o mat.o mat.c
/usr/bin/arm-linux-gnueabihf-gcc  -c -o threads_mgr.o threads_mgr.c
/usr/bin/arm-linux-gnueabihf-ld -o mysimu  kanayama.o  guidance_mgr.o  measurement_randomization.o  segment.o  simulator.o  parametres.o  mat.o  threads_mgr.o -Map mysimu.map -L/usr/include/arm-linux-gnueabihf/ -lpthread -lm -lc -e main main.o

