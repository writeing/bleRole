#include "app.h"
#include "user_main.h"
#include "string.h"
#include "stdlib.h"
#include "bleSend.h"
#include "fifo.h"
#include "bleOpt.h"

static stuBleFifo 		_stuBleFifoobj;
stuBledeviceInfo stuDebugDI;
static int appStatus = 0;
void inputRevDebug(void)
{	
	static int revDataMode = DEBUG_REV_MODE_CMD;
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
			printf("fileType = %s\r\n",stuDebugDI.filetype);
			//send data to ble
		}	
		else if(strstr(stuDebugDI.buff,"fileNum"))
		{
			sscanf(stuDebugDI.buff,"fileNum:%d;",&(stuDebugDI.localNum));	
			printf("fileNum = %d\r\n",stuDebugDI.localNum);
			//send data to ble
		}
		else if(strstr(stuDebugDI.buff,"fileEncrypt"))
		{
			char result[10];
			sscanf(stuDebugDI.buff,"fileEncrypt:%[^;]",result);	
			printf("fileEncrypt = %s\r\n",result);
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
			printf("FileObj = %d\r\n",num);
			stuDebugDI.fileObj[(num%10) - 1] = 1;
			stuDebugDI.fileObj[(num/10) - 1] = 1;	
			stuDebugDI._fileaim = num;
			
			appStatus = 1;
			revDataMode = DEBUG_REV_MODE_DATA;
		}
		memset(stuDebugDI.buff,0,100);
	}
	if(revDataMode == DEBUG_REV_MODE_DATA)
	{
		//rev data
		//set a fifo	
		stuDebugDI.nowRevCount += stuDebugDI.len;
		inputInfo(&_stuBleFifoobj,stuDebugDI.buff,stuDebugDI.len);
		if(stuDebugDI.nowRevCount == stuDebugDI.filesize)
		{
			//rev finish
			revDataMode = DEBUG_REV_MODE_CMD;
			stuDebugDI.nowRevCount = 0;
		}
	}
	
}

void _main(void)
{
	char ch;
	//init ble
	while(1)
	{
		if(getFIfoCount(&_stuBleFifoobj) > 0)
		{
			//send data to ble		
		}
		HAL_Delay(300);	
		if(appStatus == 1)
		{
						//send data to ble
				bleSendHeadInfoDevice(&stuDebugDI);
				printf("deviceBengin:2;");				
				//call pc is ok then send data
				appStatus = 0;
		}
		//printf("hello");
	}
}










