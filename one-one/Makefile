all: project sweet spin mutex signal

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

sweet.o: sweet.c
	gcc -c -g tests/sweet.c

spin.o: spint.c
	gcc -c -g tests/spin.c

mutex.o: mutex.c
	gcc -c -g tests/mutex.c

signal.o: signal.c
	gcc -c -g tests/signal.c

main.o: main.c
	gcc -c -g main.c

thread.o: thread.c thread.h
	gcc -c -g thread.c

clean:
	rm *.o tests/*.o project sweet spin mutex signal
