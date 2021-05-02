bin_PROGRAMS = project
project_SOURCES = main.c thread.c thread.h queue.c

project: main.o thread.o queue.o
	gcc -g main.o thread.o queue.o -o project

queue.o: queue.c
	gcc -c queue.c

main.o: main.c
	gcc -c main.c

thread1.o: thread.c thread.h
	gcc -c thread.c

clean:
	rm *.o project
