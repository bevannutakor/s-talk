#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "list.h"
#include "printMessage.h"

#define MAX_MSG_LEN 65506

List* messageList;

static pthread_t printMessageThread;
static pthread_mutex_t printMessageMutexCondition = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t printMessageCondition = PTHREAD_COND_INITIALIZER;

static void* printMessage(){

    pthread_mutex_lock(&printMessageMutexCondition);
    //A critical section: Do not do anything until signalled by c
    {
        pthread_cond_wait(&printMessageCondition, &printMessageMutexCondition);
    }

    pthread_mutex_unlock(&printMessageMutexCondition);

    //operations to write message to screen
    char* message;
    int messageCount;
    while(1){
        messageCount = List_count(messageList);

        if(messageCount == 0){
            //get out of the loop, and signal another thread
            break;
        }
        message = List_trim(messageList);
        printf("%s", message);
        
        message = NULL;
        
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
