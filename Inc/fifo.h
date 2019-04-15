#ifndef FIFO
#define FIFO
#include "app.h"


extern void 	createBleFifo(stuBleFifo *temp,int count);
extern void 	inputInfo(stuBleFifo *temp,char *buff,int len);
extern int 		getFifoData(stuBleFifo *temp,char *ch);
extern int 		getFIfoCount(stuBleFifo *temp);
extern void 	cleanFifo(stuBleFifo *temp);

















#endif

