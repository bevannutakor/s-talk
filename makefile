all: output

output: main.c
	gcc -o output main.c

clean:
	rm -f output