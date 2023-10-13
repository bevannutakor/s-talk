all: build

build:
	gcc -o output main.c recieveMessage.c -o s-talk -lpthread

clean:
	rm -f s-talk