all: mainRoutine

mainRoutine : src/main.c  src/jsonApi.c 
	gcc -o ./bin/mainRoutine src/main.c
