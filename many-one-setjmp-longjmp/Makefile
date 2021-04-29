bin_PROGRAMS = project
project_SOURCES = main.c thread1.c thread1.h queue.c queue.h

project: main.o thread1.o queue.o
	gcc -g main.o thread1.o queue.o -o project

queue.o: queue.c queue.h
	gcc -c queue.c 

main.o: main.c
	gcc -c main.c

thread1.o: thread1.c thread1.h
	gcc -c thread1.c

clean:
	rm *.o project
