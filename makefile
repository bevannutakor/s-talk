all: build

build:
	gcc -o output main.c messageInput.c sendMessage.c recieveMessage.c printMessage.c list.o -o s-talk -lpthread

clean:
	rm -f s-talk