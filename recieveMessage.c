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
#include "printMessage.h"
#include "messageInput.h"
#include "sendMessage.h"

#define MAX_MSG_LEN 65506

static char* myPort;
static List* messageList;
static pthread_t recieveUDPThread;

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
            close(socketInfo);
            perror("Couldn't bind socket");
            printf("\n");
            continue;
        }

        break;

    }

    //when done with everything
    freeaddrinfo(serverInfo);

    if(listIterator == NULL){
        printf("Couldn't find a network to connect too\n");
        exit(-1);
    }


    //recive message from peer
    while(1){
        //this needs to be modified so that it is getting messages from the linked list
        /*
        What should happen -> a message has been recived
                              so we insert the message into the front(head) of linked list
                              after that get a writing thread to write that message to the screen
        */
        socklen_t peerAddrLength = sizeof(peerAddress);
        int recieverBytes = recvfrom(socketInfo, buffer, MAX_MSG_LEN, 0, (struct sockaddr*) &peerAddress, &peerAddrLength);
        int terminateIdx = (recieverBytes < MAX_MSG_LEN) ? recieverBytes : MAX_MSG_LEN - 1;
        buffer[terminateIdx] = 0;

        int prepend = List_prepend(messageList, buffer);

        if(prepend == -1){
            fprintf(stderr, "There is an error with the list insertion");
        }

        //signal writer to write
        printMessageConditionSignal();

    }
}


void recieveCreateThread(char* p, List* l){
    myPort = p;
    messageList = l;

    int threadVal = pthread_create(&recieveUDPThread, NULL, recieverUDPSetup, NULL);

    if(threadVal != 0){
        perror("There was an error with the thread");
        exit(-1);
    }
}

void recieveCancelThread(){
    pthread_cancel(recieveUDPThread);
}

void recieveFinishThread(){
    pthread_join(recieveUDPThread, NULL);
}