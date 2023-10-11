all: output

output: main.c
	gcc -o output main.c recieveMessage.c

clean:
	rm -f output