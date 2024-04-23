all:
	gcc main.c -o out -Wall | ./out

clean:
	rm -rf out