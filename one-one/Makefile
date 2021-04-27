bin_PROGRAMS = project
project_SOURCES = main.c thread.c thread.h

project: main.o thread.o
	gcc -g main.o thread.o -o project

main.o: main.c
	gcc -c -g main.c

thread.o: thread.c thread.h
	gcc -c -g thread.c

clean:
	rm *.o project
