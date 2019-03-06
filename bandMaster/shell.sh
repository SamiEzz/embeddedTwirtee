arm-linux-gnueabihf-gcc  -g -c -o main.o main.c
arm-linux-gnueabihf-gcc  -g -c -o src/kanayama.o src/kanayama.c
arm-linux-gnueabihf-gcc  -g -c -o src/guidance_mgr.o src/guidance_mgr.c
arm-linux-gnueabihf-gcc  -g -c -o src/measurement_randomization.o src/measurement_randomization.c
arm-linux-gnueabihf-gcc  -g -c -o src/segment.o src/segment.c
arm-linux-gnueabihf-gcc  -g -c -o src/simulator.o src/simulator.c
arm-linux-gnueabihf-gcc  -g -c -o src/parametres.o src/parametres.c
arm-linux-gnueabihf-gcc  -g -c -o src/mat.o src/mat.c
arm-linux-gnueabihf-gcc  -g -c -o src/threads_mgr.o src/threads_mgr.c
arm-linux-gnueabihf-gcc -o mysimu  main.o  ./src/kanayama.o  ./src/guidance_mgr.o  ./src/measurement_randomization.o  ./src/segment.o  ./src/simulator.o  ./src/parametres.o  ./src/mat.o  ./src/threads_mgr.o -lpthread -lm -lc
/bin/rm *.o
echo mysimu "build done"
