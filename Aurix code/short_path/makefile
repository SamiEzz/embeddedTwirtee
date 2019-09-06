all: spf

spf : src/main.c  src/jsonApi.c src/dijkstra.c src/spf_thread.c 
	arm-linux-gnueabihf-gcc -pthread src/main.c src/jsonApi.c src/dijkstra.c src/spf_thread.c -o ./bin/spf -lm -std=gnu99