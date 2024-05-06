.PHONY: all, clean

all: main.o

main.o: main.c
	gcc main.c -o main.o -Wall -Wextra

clean:
	rm -f *.o