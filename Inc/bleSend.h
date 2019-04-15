#ifndef BLESEND
#define BLESEND
#include "app.h"



extern void 	bleSendHeadInfoDevice(stuBledeviceInfo *temp);
extern void 	serialSendHeadInfoPc(stuBledeviceInfo *temp);
extern int 		serialSendBuffPc(char *buff,int len);
extern int 		bleSendBuffDevice(char *buff,int len);
extern void 	sendBuffToBleModule(char *buff ,int len);
























#endif

