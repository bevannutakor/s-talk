#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <netinet/in.h>

#include "list.h"

#define MAX_MSG_LEN 1024

static char* myPort;
static List* messageList;
static pthread_t recieveThread;

static void recieverUDPSetup()
{
    //initialize network variables
    int socketInfo;
    int socketBind;
    struct addrinfo hints, *serverInfo, *listIterator;
    struct sockaddr_in peerAddress;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int addrStatus = getaddrinfo(NULL, myPort, &hints, &serverInfo);

    if(addrStatus != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addrStatus));
        exit(1); //does the number matter?
    }

    //get all ip addresses on user network and create a socket with one of them
    for(listIterator = serverInfo; listIterator != NULL; listIterator = serverInfo->ai_next){
        socketInfo = socket(listIterator->ai_family, listIterator->ai_socktype, listIterator->ai_protocol);

        if(socketInfo == -1){
            perror("Couldn't create socket");
            continue;
        }

        socketBind = bind(socketInfo, listIterator->ai_addr, listIterator->ai_addrlen); //create connection

        if(socketBind == -1){
            perror("Couldn't bind socket");
            continue;
        }

        break;

    }

    //when done with everything
    freeaddrinfo(serverInfo);

    if(listIterator == NULL){
        printf("Couldn't find a network to connect too");
        exit(-1);
    }


    //recive message from peer
    while(1){
        //do stuff

    }

    //when done with everything
    //freeaddrinfo(serverInfo);
}


void recieveCreateThread(char* myPort, List* messageList)
{

}

void recieveCancelThread()
{

}

void recieveFinishThread()
{

}