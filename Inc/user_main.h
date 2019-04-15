#ifndef USER_MAIN
#define USER_MAIN
#include "main.h"


extern int revBuffSize;
extern char revBuff[128];
void initFifo(void);
ErrorStatus getDebugData(char *buff,int *len);




#endif
