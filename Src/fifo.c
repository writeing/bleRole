#include "fifo.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "main.h"
#if 1
/*********************************************************/
void createBleFifo(stuBleFifo *temp,int count)
{
	if(temp->count != 0)
		cleanFifo(temp);
	temp->count = 1025;
	temp->writeIndex = 0;
	temp->readIndex = 0;
	temp->curCount = 0;
	temp->g_timeFlag = 0;
}
	
void inputInfo(stuBleFifo *temp,char *buff,int len)
{	
	//get signal
	memcpy(temp->buff,buff,len);		
	temp->curCount = len;
}

void inputCharInfo(stuBleFifo *temp,char ch)
{
	if(temp->g_timeFlag == 0)
	{
		temp->g_timeFlag = HAL_GetTick();
	}
	temp->buff[temp->curCount] = ch;
	temp->curCount++;
}
void printInfo(stuBleFifo *temp)
{
//	int len = 0;
//	if(g_timeFlag != 0 && HAL_GetTick() - g_timeFlag > 10)
//	{
//		//had start and delay
//		memset(buff,0,1024);
//		memcpy(buff,temp->buff,temp->curCount);
//		len = temp->curCount;
//		temp->curCount = 0;
//		
//		printf("%s",buff);	
//		g_timeFlag = 0;
//	}
}

int getFifoData(stuBleFifo *temp,char *data)
{
	int len = 0;

	if(temp->g_timeFlag != 0 && HAL_GetTick() - temp->g_timeFlag > 100)
	{
		//printf("%d\r\n",g_timeFlag);
		memset(data,0,1024);
		memcpy(data,temp->buff,temp->curCount);
		len = temp->curCount;
		temp->curCount = 0;
		temp->g_timeFlag = 0;		
	}
	return len;
}
int getFIfoCount(stuBleFifo *temp)
{
	return temp->curCount;
}

void cleanFifo(stuBleFifo *temp)
{
	memset(temp->buff,0,2048);
	temp->count = 0;
	temp->g_timeFlag = 0;
}
/*********************************************************/
#endif

























