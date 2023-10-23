#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include "recieveMessage.h"
#include "printMessage.h"
#include "messageInput.h"
#include "sendMessage.h"
#include "list.h"

#define MAX_STR_LEN 1000

int main(int argc, char *argv[]){
    //need to run this in command line -> s-talk [myport] [remote machine name] [remote port number]
    //define variables

    if( argc == 4 ) {
        char* myPort = argv[1];
        char* remoteMachineName = argv[2];
        char* remotePort = argv[3];

        List* messageList = List_create();
        //all threads
        messageInputCreateThread(messageList);
        sendMessageCreateThread(remoteMachineName, remotePort, messageList);
        recieveCreateThread(myPort, messageList);
        printMessageCreateThread(messageList);


        //thread finishing
        messageInputFinishThread();
        sendMessageFinishThread();
        recieveFinishThread();
        printMessageFinishThread();
        
    } else {
        printf("This is incorrect please input 4 arguments");
    }

    return 0;
}