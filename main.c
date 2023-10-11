#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define MAX_STR_LEN 1000

int main(int argc, char *argv[]){
    //need to run this in command line -> s-talk [myport] [remote machine name] [remote port number]
    //define variables

    if( argc == 4 ) {
        char* myPort = argv[1];
        char* remoteMachineName = argv[2];
        char* remotePort = argv[3];

        printf("The port: %s\n", myPort);
        printf("The remote machine: %s\n", remoteMachineName);
        printf("The remote port: %s\n", remotePort);
    } else {
        printf("This is incorrect please input 4 arguments");
    }

    return 0;
}