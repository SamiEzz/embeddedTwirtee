all: 	unit_test_kalman

unit_test_kalman: parametres.o mat.o kalman.o main.o 
		gcc -o unit_test_kalman parametres.o mat.o kalman.o 
kalman.o: kalman.o mat.o
		gcc -o kalman.o -c kalman.c
mat.o : mat.c
		gcc -o mat.o -c  mat.c -lm
parametres.o: parametres.c
		gcc -o parametres.o -c parametres.c
main.o : main.c
		gcc -o main.o -c main.c
clean:
	rm -rf *.o
