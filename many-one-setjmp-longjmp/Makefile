all: project sweet spin mutex signal stress matrix stress2 create join

project: main.o thread.o queue.o
	gcc -g main.o thread.o queue.o -o project

sweet: tests/sweet.o thread.o queue.o
	gcc -g tests/sweet.o thread.o queue.o -o sweet

create: tests/create.o thread.o queue.o
	gcc -g tests/create.o thread.o queue.o -o create

join: tests/join.o thread.o queue.o
	gcc -g tests/join.o thread.o queue.o -o join

spin: tests/spin.o thread.o queue.o
	gcc -g tests/spin.o thread.o queue.o -o spin

mutex: tests/mutex.o thread.o queue.o
	gcc -g tests/mutex.o thread.o queue.o -o mutex

signal: tests/signal.o thread.o queue.o
	gcc -g tests/signal.o thread.o queue.o -o signal

stress: tests/stress.o thread.o queue.o
	gcc -g tests/stress.o thread.o queue.o -o stress

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

mutex.o: mutex.c
	gcc -c -g tests/mutex.c

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

clean:
	rm *.o tests/*.o project sweet spin mutex signal stress matrix stress2 create join

# bin_PROGRAMS = project
# project_SOURCES = main.c thread.c thread.h queue.c

# project: main.o thread.o queue.o
# 	gcc -g main.o thread.o queue.o -o project

# queue.o: queue.c
# 	gcc -g -c queue.c

# main.o: main.c
# 	gcc -g -c main.c

# thread.o: thread.c thread.h
# 	gcc -g -c thread.c

# clean:
# 	rm *.o project
