#include "bleOpt.h"
#include "app.h"
#include "main.h"
#include "fifo.h"
#include "string.h"
#include "user_main.h"
#include "FIFO_Uart.h"
#include "usart.h"

extern UART_FIFO_Typedef_t usart_ble;


static stuBleFifo 		_stuBleFifoobj;
stuBledeviceInfo stubleDI;

void inputRevBle(void)
{	
	static int revDataMode = BLE_MODULE_MODE_CMD;
	if(revDataMode == BLE_REV_MODE_CMD)
	{
		if(strstr(stubleDI.buff,"fileName") != NULL)
		{
			stubleDI.nowRevCount = 0;
			sscanf(stubleDI.buff,"fileName:%s;",stubleDI.filename);
			printf("fileName = %s\r\n",stubleDI.filename);
			//send data to pc
			printf("fileName:%s;",stubleDI.filename);
		}
		else if(strstr(stubleDI.buff,"fileSize"))
		{
			sscanf(stubleDI.buff,"fileSize:%d;",&(stubleDI.filesize));	
			printf("fileSize = %d\r\n",stubleDI.filesize);			
			createBleFifo(_stuBleFifoobj,stubleDI.filesize);
			//send data to pc
			printf("fileSize:%d;",stubleDI.filesize);
		}
		else if(strstr(stubleDI.buff,"fileType"))
		{
			sscanf(stubleDI.buff,"fileType:%s;",stubleDI.filetype);	
			printf("fileType = %s\r\n",stubleDI.filetype);
			//send data to pc
			printf("fileType:%s;",stubleDI.filetype);
		}	
		else if(strstr(stubleDI.buff,"fileNum"))
		{
			sscanf(stubleDI.buff,"fileNum:%d;",&(stubleDI.aimNum));	
			printf("fileNum = %d\r\n",stubleDI.aimNum);
			//send data to pc
			printf("fileNum:%d;",stubleDI.aimNum);			
		}
		else if(strstr(stubleDI.buff,"fileEncrypt"))
		{
			char result[10];
			sscanf(stubleDI.buff,"fileEncrypt:%s;",result);	
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
		else if(strstr(stubleDI.buff,"deviceBegin"))
		{
			//sscanf(stubleDI.buff,"fileNum:%d;",&(stubleDI.aimNum));	
			//printf("fileNum = %d\r\n",stubleDI.aimNum);
			//send data to pc
			printf("deviceBengin:1;");	
			revDataMode = DEBUG_REV_MODE_DATA;
		}
	}
	if(revDataMode == BLE_REV_MODE_DATA)
	{
		//rev data
		//set a fifo		
		stubleDI.nowRevCount += stubleDI.len;
		inputInfo(_stuBleFifoobj,stubleDI.buff,stubleDI.len);
		if(stubleDI.nowRevCount == stubleDI.filesize)
		{
			//rev finish
			revDataMode = DEBUG_REV_MODE_CMD;
			stubleDI.nowRevCount = 0;
		}
	}
	if(revDataMode == BLE_MODULE_MODE_CMD)
	{
		//check at cmd and check connect status
	}
}

ErrorStatus getBleData(char *buff,int *len)
{
	uint8_t aRxBuffer;
	*len = 0;
	while(FIFO_UartReadByte(&usart_ble,&aRxBuffer) == HAL_OK)	
	{
		buff[*len] = aRxBuffer;		
		(*len) ++;
	}
	return SUCCESS;
}



















