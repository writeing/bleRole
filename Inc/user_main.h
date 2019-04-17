#ifndef USER_MAIN
#define USER_MAIN
#include "main.h"


extern int revBuffSize;
extern char revBuff[128];
void initFifo(void);
extern ErrorStatus 		getDebugData(char *buff,int *len);
extern ErrorStatus 		checkRevData(int num);
extern void 			sendBuffToPc(uint8_t *buff,int len);

#endif
