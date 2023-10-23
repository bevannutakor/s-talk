#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "list.h"
#include "sendMessage.h"
#include "recieveMessage.h"

#define MAX_MSG_LEN 65506

static pthread_t messageInputThread;
static List* messageList;

static void* messageInput(){

    while(1){
        char inputBuffer[MAX_MSG_LEN];
        char* message;
        int actualMessageSize;

        //printf("Write your message: \n");
        if(fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL){
            //printf("%s\n", inputBuffer);
            size_t actualMessageSize = strlen(inputBuffer);

            message = (char*)malloc(sizeof(char)*actualMessageSize+1);
            strncpy(message, inputBuffer, actualMessageSize);

            message[actualMessageSize-1] = '\0';
            int addMessage = List_prepend(messageList, message);

            if(addMessage == -1){
                perror("Something wen wrong with adding a message");
                exit(-1);
            }

            
            //after we have loaded our input we should signal our sender
            sendMessageSignal();
        }
    }

}

void messageInputCreateThread(List* list){
    messageList = list;

    int threadVal = pthread_create(&messageInputThread, NULL, messageInput, NULL);
    if(threadVal != 0){
        perror("Could not create message input read thread");
        exit(-1);
    }
}

void messageInputCancelThread(){
    pthread_cancel(messageInputThread);
}

void messageInputFinishThread(){
    pthread_join(messageInputThread, NULL);
}