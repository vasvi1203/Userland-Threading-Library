all: project sweet spin mutex signal stress matrix

project: main.o thread.o
	gcc -g main.o thread.o -o project

sweet: tests/sweet.o thread.o
	gcc -g tests/sweet.o thread.o -o sweet

spin: tests/spin.o thread.o
	gcc -g tests/spin.o thread.o -o spin

mutex: tests/mutex.o thread.o
	gcc -g tests/mutex.o thread.o -o mutex

signal: tests/signal.o thread.o
	gcc -g tests/signal.o thread.o -o signal

stress: tests/stress.o thread.o
	gcc -g tests/stress.o thread.o -o stress

matrix: tests/matrix.o thread.o
	gcc -g tests/matrix.o thread.o -o matrix

sweet.o: sweet.c
	gcc -c -g tests/sweet.c

spin.o: spint.c
	gcc -c -g tests/spin.c

mutex.o: mutex.c
	gcc -c -g tests/mutex.c

signal.o: signal.c
	gcc -c -g tests/signal.c

stress.o: stress.c
	gcc -c -g tests/stress.c

matrix.o: matrix.c
	gcc -c -g tests/matrix.c


main.o: main.c
	gcc -c -g main.c

thread.o: thread.c thread.h
	gcc -c -g thread.c

clean:
	rm *.o tests/*.o project sweet spin mutex signal stress matrix
