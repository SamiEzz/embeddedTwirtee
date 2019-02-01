all: spf

spf : src/main.c  src/jsonApi.c src/dijkstra.c src/spf_thread.c 
	gcc -pthread src/main.c src/jsonApi.c src/dijkstra.c src/spf_thread.c -o ./bin/spf -lm


