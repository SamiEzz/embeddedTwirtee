all: bandmaster

bandmaster : main.c 
	gcc -o ./bin/bandmaster main.c -lpthread
