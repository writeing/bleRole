#include "bleOpt.h"
#include "app.h"
#include "main.h"
#include "fifo.h"
#include "string.h"
#include "user_main.h"
#include "FIFO_Uart.h"
#include "usart.h"

stuBleFifo 		blestuBleFifoobj;
stuBledeviceInfo stubleDI;
int revBleDataMode = BLE_REV_MODE_CMD;
static int isAim = 0;
char gs_chbuff[1024] = {0};
char gs_chIndex = 0;

void inputRevBle(void)
{
	if(revBleDataMode == BLE_REV_MODE_CMD)
	{
		//printf("%s\r\n",stubleDI.buff);
		if(strstr(stubleDI.buff,"ble"))
		{

			stubleDI.nowRevCount = 0;
			sscanf(stubleDI.buff,"ble:%d;%d;%d;%d;%[^;]",&(stubleDI.filesize),&(stubleDI.aimNum),&(stubleDI.fileEncrypt),&(stubleDI._fileaim),stubleDI.filename);
			//printf("filesize = %d\r\n",stubleDI.filesize);
			if(checkRevData(stubleDI._fileaim) == SUCCESS)
			{
				//can rev
				isAim = 1;
				printf("fileName:%s;",stubleDI.filename);
				printf("fileSize:%d;",stubleDI.filesize);
				printf("fileNum:%d;",stubleDI.aimNum);
				//printf("is will rev data\r\n");
			}
			else
			{
				//can`t rev
				isAim = 0;
			}
		}
		else if(strstr(stubleDI.buff,"deviceBegin"))
		{
			//send data to pc
			printf("deviceBengin:1;");	
			if(isAim)
			{
				revBleDataMode = BLE_REV_MODE_DATA;
				isAim = 0;
			}			
		}
		else if(strstr(stubleDI.buff,"fileName") != NULL)
		{

			stubleDI.nowRevCount = 0;
			sscanf(stubleDI.buff,"fileName:%[^;]",stubleDI.filename);
			//printf("fileName = %s\r\n",stubleDI.filename);
			//send data to pc
			printf("fileName:%s;",stubleDI.filename);
		}
		else if(strstr(stubleDI.buff,"fileSize"))
		{
			sscanf(stubleDI.buff,"fileSize:%d;",&(stubleDI.filesize));	
			//printf("fileSize = %d\r\n",stubleDI.filesize);			
			createBleFifo(&blestuBleFifoobj,stubleDI.filesize);
			//send data to pc
			printf("fileSize:%d;",stubleDI.filesize);
		}
		else if(strstr(stubleDI.buff,"fileType"))
		{
			sscanf(stubleDI.buff,"fileType:%[^;]",stubleDI.filetype);	
		  //printf("fileType = %s\r\n",stubleDI.filetype);
			//send data to pc
			printf("fileType:%s;",stubleDI.filetype);
		}	
		else if(strstr(stubleDI.buff,"fileNum"))
		{
			sscanf(stubleDI.buff,"fileNum:%d;",&(stubleDI.aimNum));	
			//printf("fileNum = %d\r\n",stubleDI.aimNum);
			//send data to pc
			printf("fileNum:%d;",stubleDI.aimNum);			
		}
		else if(strstr(stubleDI.buff,"fileEncrypt"))
		{
			char result[10];
			sscanf(stubleDI.buff,"fileEncrypt:%[^;]",result);	
			printf("fileEncrypt = %s\r\n",result);
			//false //true
			if(strstr(result,"false")!= NULL)
			{
				stubleDI.fileEncrypt = 0;
			}
			else
			{
				stubleDI.fileEncrypt = 1;
			}
		}
		else if(strstr(stubleDI.buff,"FileObj"))
		{
			int num = 0;
			sscanf(stubleDI.buff,"FileObj:%d;",&num);	
			stubleDI._fileaim = num;		
			printf("FileObj=%d\r\n",num);			
			//check aim is local
			if(checkRevData(num) == SUCCESS)
			{
				//can rev
				isAim = 1;
				printf("is will rev data\r\n");
			}
			else
			{
				//can`t rev
				isAim = 0;
			}
		}
		
		memset(stubleDI.buff,0,100);
		stubleDI.len = 0;
	}
	else if(revBleDataMode == BLE_REV_MODE_DATA )			
	{
			
//		//copy data to buff
//		memcpy(gs_chbuff,stubleDI.buff,stubleDI.len);
//		gs_chIndex = stubleDI.len;
		
	}
	else if(revBleDataMode == BLE_MODULE_MODE_CMD)
	{
		//check at cmd and check connect status
		
	}
	else
	{
		//
	}
}


void checkBleData(void)
{	
	if(stubleDI.len  > 0 && revBleDataMode == BLE_REV_MODE_DATA)
	{
		//printf("time Tick = %d,%d\r\n",HAL_GetTick() - timet,HAL_GetTick());
		stubleDI.nowRevCount += stubleDI.len;
		sendBuffToPc((uint8_t *)stubleDI.buff,stubleDI.len);
		//printf("rev %d:%d:%d\r\n",stubleDI.len,stubleDI.nowRevCount,stubleDI.filesize);
		//send data to pc
		
		if(stubleDI.nowRevCount == stubleDI.filesize)
		{
			//finish 
			//printf("deviceBengin:3;");
			revBleDataMode = BLE_REV_MODE_CMD;				
			stubleDI.nowRevCount = 0;
		}
		if(stubleDI.nowRevCount > stubleDI.filesize)
		{
			//printf("revError:3;");
			revBleDataMode = BLE_REV_MODE_CMD;				
			stubleDI.nowRevCount = 0;
		}
		stubleDI.len = 0;
	}
}


