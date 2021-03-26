bin_PROGRAMS = project
project_SOURCES = main.c thread.c thread.h

project: main.o thread.o
	gcc main.o thread.o -o project

main.o: main.c
	gcc -c main.c

thread.o: thread.c thread.h
	gcc -c thread.c

clean:
	rm *.o project
