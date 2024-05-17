.PHONY: all, clean

all: main

main: main.c lib/str_coll.c
	gcc -o main main.c lib/str_coll.c -Wall -Wextra

clean:
	rm -f *.o