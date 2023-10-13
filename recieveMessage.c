#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "list.h"

#define MAX_MSG_LEN 1024

static char* myPort;
static List* messageList;
static pthread_t recieveThread;

static void* recieverUDPSetup()
{
    //initialize network variables
    int socketInfo;
    int socketBind;
    struct addrinfo hints, *serverInfo, *listIterator;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in peerAddress;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int addrStatus = getaddrinfo(NULL, myPort, &hints, &serverInfo);

    if(addrStatus != 0){
        printf("My PORT: %s\n: ", myPort);
        fprintf(stderr, "getaddrinfo ERROR: %s\n", gai_strerror(addrStatus));
        exit(1);
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
        //this needs to be modified so that it is getting messages from the linked list
        socklen_t peerAddrLength = sizeof(peerAddress);
        int reciever = recvfrom(socketInfo, buffer, MAX_MSG_LEN, 0, (struct sockaddr*) &peerAddress, &peerAddrLength);
        int terminateIdx = (reciever < MAX_MSG_LEN) ? reciever : MAX_MSG_LEN - 1;
        buffer[terminateIdx] = 0;

        printf("%s", buffer);
    }
}


void recieveCreateThread(char* p)
{
    myPort = p;
    //recieverUDPSetup();

    //messageList = messageList;
    int threadVal = pthread_create(&recieveThread, NULL, recieverUDPSetup, NULL);

    if(threadVal != 0){
        perror("There was an error with the thread");
        exit(-1);
    }
}

void recieveCancelThread()
{

}

void recieveFinishThread()
{
    pthread_join(recieveThread, NULL);
}