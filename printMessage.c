#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "list.h"
#include "printMessage.h"
#include "recieveMessage.h"
#include "sendMessage.h"
#include "messageInput.h"

#define MAX_MSG_LEN 65506

List* messageList;

static pthread_t printMessageThread;
static pthread_mutex_t printMessageMutexCondition = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t printMessageCondition = PTHREAD_COND_INITIALIZER;

static void* printMessage(){

    pthread_mutex_lock(&printMessageMutexCondition);
    //Do not do anything until signalled
    {
        pthread_cond_wait(&printMessageCondition, &printMessageMutexCondition);
    }

    pthread_mutex_unlock(&printMessageMutexCondition);

    //operations to write message to screen
    char* message;
    int messageCount;
    messageCount = List_count(messageList);
    while(1){
        //printf("Incoming message!: \n");
        message = List_trim(messageList);
        
        printf("%s\n", message);

        char endMessage[2];
        endMessage[0] = '!';
        endMessage[1] = '\0';
        int compareResult = strcmp(message, endMessage);

        if(compareResult == 0){
            messageInputCancelThread();
            sendMessageCancelThread();
            recieveCancelThread();
            break;
        }
        
        message = NULL;
        messageCount = List_count(messageList);
        if(messageCount == 0){
            //here we lock it instead of canceling the entire loop
            pthread_mutex_lock(&printMessageMutexCondition);
            {
                pthread_cond_wait(&printMessageCondition, &printMessageMutexCondition);
            }

            pthread_mutex_unlock(&printMessageMutexCondition);
        }
    }
}


void printMessageConditionSignal(){
    pthread_mutex_lock(&printMessageMutexCondition);

    {
        pthread_cond_signal(&printMessageCondition);
    }

    pthread_mutex_unlock(&printMessageMutexCondition);
}

void printMessageCreateThread(List* list){
    //create thread
    messageList = list;

    int threadVal = pthread_create(&printMessageThread, NULL, printMessage, NULL);

    if(threadVal != 0){
        perror("There was an error creating the thread");
        exit(-1);
    }
}

void printMessageCancelThread(){
    pthread_cancel(printMessageThread);
}

void printMessageFinishThread(){
    pthread_join(printMessageThread, NULL);
}
