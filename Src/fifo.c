#include "fifo.h"
#include "string.h"
#include "stdlib.h"

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
}
	
void inputInfo(stuBleFifo *temp,char *buff,int len)
{
	//memcpy(temp->buff+,buff,len);
	for(int i = 0 ; i < len ; i ++)
	{
		temp->buff[temp->writeIndex] = buff[i];
		temp->writeIndex ++;
		temp->writeIndex = temp->writeIndex % temp->count;			
	}
	temp->curCount += len;
}

int getFifoData(stuBleFifo *temp,char *ch)
{
	if(temp->readIndex == temp->writeIndex)
		return 0;
	*ch = temp->buff[temp->readIndex];
	temp->readIndex ++;
	temp->readIndex %= temp->count;
	temp->curCount --;
	return 1;
}
int getFIfoCount(stuBleFifo *temp)
{
	return temp->curCount;
}

void cleanFifo(stuBleFifo *temp)
{
	memset(temp->buff,0,2048);
	temp->count = 0;
	temp->writeIndex = 0;
	temp->readIndex = 0;
}
/*********************************************************/
#endif

























