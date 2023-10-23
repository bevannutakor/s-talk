#include "list.h"

void sendMessageSignal();
void sendMessageCreateThread(char* pHost, char* pPort, List* list);
void sendMessageCancelThread();
void sendMessageFinishThread();