all: project sweet spin signal stress matrix stress2 create join

run:
	bash test.sh

project: main.o thread.o queue.o
	gcc -g main.o thread.o  queue.o -o project

queue.o: queue.c
	gcc -c queue.c

sweet: tests/sweet.o thread.o queue.o
	gcc -g tests/sweet.o thread.o queue.o -o sweet

create: tests/create.o thread.o queue.o
	gcc -g tests/create.o thread.o queue.o -o create

join: tests/join.o thread.o queue.o
	gcc -g tests/join.o thread.o queue.o -o join

spin: tests/spin.o thread.o queue.o lock.o
	gcc -g tests/spin.o thread.o queue.o lock.o -o spin

signal: tests/signal.o thread.o queue.o
	gcc -g tests/signal.o thread.o queue.o -o signal

stress: tests/stress.o thread.o queue.o lock.o
	gcc -g tests/stress.o thread.o queue.o lock.o -o stress

stress2: tests/stress2.o thread.o queue.o
	gcc -g tests/stress2.o thread.o queue.o -o stress2

matrix: tests/matrix.o thread.o queue.o
	gcc -g tests/matrix.o thread.o queue.o -o matrix

sweet.o: sweet.c 
	gcc -c -g tests/sweet.c

create.o: create.c
	gcc -c -g tests/create.c

join.o: join.c
	gcc -c -g tests/join.c

spin.o: spint.c
	gcc -c -g tests/spin.c

signal.o: signal.c
	gcc -c -g tests/signal.c

stress.o: stress.c
	gcc -c -g tests/stress.c

stress2.o: stress2.c
	gcc -c -g tests/stress2.c

matrix.o: matrix.c
	gcc -c -g tests/matrix.c


main.o: main.c
	gcc -c -g main.c

thread.o: thread.c thread.h
	gcc -c -g thread.c


lock.o: lock.c
	gcc -c -g lock.c


clean:
	rm *.o tests/*.o project sweet spin signal stress matrix stress2 create join
