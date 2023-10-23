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

#define MAX_MSG_LEN 65506

static pthread_t sendMessageThread;
static pthread_mutex_t sendMessageConditionMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sendMessageCondition = PTHREAD_COND_INITIALIZER;

static char* peerHost;
static char* peerPort;

static List* messageList;

static void* sendUDPSetup(){
    int socketInfo;
    char* message;
    struct addrinfo hints, *serverInfo, *listIterator;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int addrStatus = getaddrinfo(peerHost, peerPort, &hints, &serverInfo);

    if(addrStatus != 0){
        fprintf(stderr, "getaddrinfo ERROR: %s\n", gai_strerror(addrStatus));
        exit(1);
    }


    //here we get a working ip addresses on peer host
    for(listIterator = serverInfo; listIterator != NULL; listIterator = serverInfo->ai_next){
        socketInfo = socket(listIterator->ai_family, listIterator->ai_socktype, listIterator->ai_protocol);

        if(socketInfo == -1){
            perror("Couldn't create socket");
            continue;
        }

        break;
    }

    freeaddrinfo(serverInfo);

    if(listIterator == NULL){
        printf("Couldn't find a network to connect too");
        exit(-1);
    }

    int sendTo;
    int messageCount = List_count(messageList);
    int messageLength;
    while(1){
        //when thread is signaled to start, run code to send message
        pthread_mutex_lock(&sendMessageConditionMutex);

        {
            pthread_cond_wait(&sendMessageCondition, &sendMessageConditionMutex);
        }

        pthread_mutex_unlock(&sendMessageConditionMutex);

        printf("testing:\n");
        message = List_remove(messageList);
        
        messageLength = strlen(message);

        //send message
        sendTo = sendto(socketInfo, message, messageLength, 0, listIterator->ai_addr, listIterator->ai_addrlen);
    
        messageCount = List_count(messageList);

        while(messageCount != 0);
    }
}


void sendMessageSignal(){
    pthread_mutex_lock(&sendMessageConditionMutex);

    {
        pthread_cond_signal(&sendMessageCondition);
    }

    pthread_mutex_unlock(&sendMessageConditionMutex);
}

void sendMessageCreateThread(char* pHost, char* pPort, List* list){
    peerHost = pHost;
    peerPort = pPort;
    messageList = list;

    int threadVal = pthread_create(&sendMessageThread, NULL, sendUDPSetup, NULL);

    if(threadVal != 0){
        perror("There was an error creating the send message thread");
        exit(-1);
    }
}

void sendMessageCancelThread(){
    pthread_cancel(sendMessageThread);
}

void sendMessageFinishThread(){
    pthread_join(sendMessageThread, NULL);
}