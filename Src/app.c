#include "app.h"
#include "user_main.h"
#include "string.h"
#include "stdlib.h"
#include "bleSend.h"
#include "fifo.h"
#include "bleOpt.h"
#include "FIFO_Uart.h"


stuBleFifo 		_stuBleFifoobj;
stuBledeviceInfo stuDebugDI;
static int appStatus = 0;
int revDataMode = DEBUG_REV_MODE_CMD;

static char gs_chbuff[1024] = {0};
static char gs_chIndex = 0;
void inputRevDebug(void)
{	

	if(revDataMode == DEBUG_REV_MODE_CMD)
	{
		if(strstr(stuDebugDI.buff,"fileName") != NULL)
		{
			stuDebugDI.nowRevCount = 0;
			sscanf(stuDebugDI.buff,"fileName:%[^;]",stuDebugDI.filename);
			printf("fileName = %s\r\n",stuDebugDI.filename);
			//send data to ble			
		}
		else if(strstr(stuDebugDI.buff,"fileSize"))
		{
			sscanf(stuDebugDI.buff,"fileSize:%d;",&(stuDebugDI.filesize));	
			printf("fileSize = %d\r\n",stuDebugDI.filesize);			
			createBleFifo(&_stuBleFifoobj,stuDebugDI.filesize);
			//send data to ble
		}
		else if(strstr(stuDebugDI.buff,"fileType"))
		{
			sscanf(stuDebugDI.buff,"fileType:%[^;]",stuDebugDI.filetype);	
			//printf("fileType = %s\r\n",stuDebugDI.filetype);
			//send data to ble
		}	
		else if(strstr(stuDebugDI.buff,"fileNum"))
		{
			sscanf(stuDebugDI.buff,"fileNum:%d;",&(stuDebugDI.localNum));	
			//printf("fileNum = %d\r\n",stuDebugDI.localNum);
			//send data to ble
		}
		else if(strstr(stuDebugDI.buff,"fileEncrypt"))
		{
			char result[10];
			sscanf(stuDebugDI.buff,"fileEncrypt:%[^;]",result);	
			//printf("fileEncrypt = %s\r\n",result);
			//false //true
			if(strstr(result,"false")!= NULL)
			{
				stuDebugDI.fileEncrypt = 0;
			}
			else
			{
				stuDebugDI.fileEncrypt = 1;
			}
			//send data to ble
		}
		else if(strstr(stuDebugDI.buff,"FileObj"))
		{
			int num = 0;
			sscanf(stuDebugDI.buff,"FileObj:%d;",&num);	
			stuDebugDI._fileaim = num;	
			//cmd rev finish
			appStatus = 1;
			revDataMode = DEBUG_REV_MODE_DATA;
		}
		stuDebugDI.len = 0;
		memset(stuDebugDI.buff,0,100);
	}
	else if(revDataMode == DEBUG_REV_MODE_DATA)
	{
		//copy data to buff
		//memcpy(gs_chbuff,stuDebugDI.buff,stuDebugDI.len);
		//gs_chIndex = stuDebugDI.len;
		
	}
}

void checkDebugData(void)
{	
	if(stuDebugDI.len > 0 && revDataMode == DEBUG_REV_MODE_DATA)
	{
		//printf("time Tick = %d,%d\r\n",HAL_GetTick() - timet,HAL_GetTick());
		stuDebugDI.nowRevCount += stuDebugDI.len;
		printf("rev %d:%d:%d\r\n",stuDebugDI.len,stuDebugDI.nowRevCount,stuDebugDI.filesize);
		//send data to ble			
		sendBuffToBleModule(stuDebugDI.buff,stuDebugDI.len);
		if(stuDebugDI.nowRevCount == stuDebugDI.filesize)
		{
			//finish 
			printf("deviceBengin:3;");
			revDataMode = DEBUG_REV_MODE_CMD;				
		}
		if(stuDebugDI.nowRevCount > stuDebugDI.filesize)
		{
			printf("revError:3;");
			revDataMode = DEBUG_REV_MODE_CMD;				
		}
		stuDebugDI.len = 0;
	}
}


void _main(void)
{

	//init ble
	cleanFifo(&_stuBleFifoobj);
	while(1)
	{			
		checkDebugData();
		checkBleData();
		if(appStatus == 1)
		{
			//send data to ble
			bleSendHeadInfoDevice(&stuDebugDI);
			printf("deviceBengin:2;");				
			//call pc is ok then send data
			appStatus = 0;
		}
	}
}










