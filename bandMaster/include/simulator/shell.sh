arm-linux-gnueabihf-gcc  -g -c -o main.o main.c
arm-linux-gnueabihf-gcc  -g -c -o kanayama.o kanayama.c
arm-linux-gnueabihf-gcc  -g -c -o guidance_mgr.o guidance_mgr.c
arm-linux-gnueabihf-gcc  -g -c -o measurement_randomization.o measurement_randomization.c
arm-linux-gnueabihf-gcc  -g -c -o segment.o segment.c
arm-linux-gnueabihf-gcc  -g -c -o simulator.o simulator.c
arm-linux-gnueabihf-gcc  -g -c -o parametres.o parametres.c
arm-linux-gnueabihf-gcc  -g -c -o mat.o mat.c
arm-linux-gnueabihf-gcc  -g -c -o threads_mgr.o threads_mgr.c
arm-linux-gnueabihf-gcc -o mysimu  main.o  kanayama.o  guidance_mgr.o  measurement_randomization.o  segment.o  simulator.o  parametres.o  mat.o  threads_mgr.o -L/usr/include/arm-linux-gnueabihf/ -lpthread -lm -lc
rm -f *.o
echo mysimu "build done"
