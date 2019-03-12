gcc  -g -c -o main.o main.c
gcc  -g -c -o src/kanayama.o src/kanayama.c
gcc  -g -c -o src/guidance_mgr.o src/guidance_mgr.c
gcc  -g -c -o src/measurement_randomization.o src/measurement_randomization.c
gcc  -g -c -o src/segment.o src/segment.c
gcc  -g -c -o src/simulator.o src/simulator.c
gcc  -g -c -o src/parametres.o src/parametres.c
gcc  -g -c -o src/mat.o src/mat.c
gcc  -g -c -o src/threads_mgr.o src/threads_mgr.c
gcc  -g -c -o src/spf_thread.o src/spf_thread.c
gcc  -g -c -o src/dijkstra.o src/dijkstra.c
gcc  -g -c -o src/jsonApi.o src/jsonApi.c
gcc  -g -c -o src/misc.o src/misc.c
gcc -o mysimu  main.o  ./src/kanayama.o  ./src/guidance_mgr.o  ./src/measurement_randomization.o  ./src/segment.o  ./src/simulator.o  ./src/parametres.o  ./src/mat.o  ./src/threads_mgr.o  ./src/spf_thread.o  ./src/dijkstra.o  ./src/jsonApi.o  ./src/misc.o -lpthread -lm -lc
echo mysimu "build done"
mv ./src/*.o ./build/
mv *.o ./build/
