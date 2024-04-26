.PHONY: all

all: main

main: main.o
	gcc -o main main.o

main.o: main.c
	gcc main.c -o main.o -Wall -Wextra

.PHONY: clean
clean:
	rm -f *.o main