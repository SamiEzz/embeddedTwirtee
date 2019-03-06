/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o main.o main.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/kanayama.o src/kanayama.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/guidance_mgr.o src/guidance_mgr.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/measurement_randomization.o src/measurement_randomization.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/segment.o src/segment.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/simulator.o src/simulator.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/parametres.o src/parametres.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/mat.o src/mat.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/threads_mgr.o src/threads_mgr.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/spf_thread.o src/spf_thread.c
/usr/bin/arm-linux-gnueabihf-gcc  -g -c -o src/dijkstra.o src/dijkstra.c
/usr/bin/arm-linux-gnueabihf-gcc -o mysimu  main.o  ./src/kanayama.o  ./src/guidance_mgr.o  ./src/measurement_randomization.o  ./src/segment.o  ./src/simulator.o  ./src/parametres.o  ./src/mat.o  ./src/threads_mgr.o  ./src/spf_thread.o  ./src/dijkstra.o -lpthread -lm -lc
echo mysimu "build done"
mv ./src/*.o ./build/
mv *.o ./build/
