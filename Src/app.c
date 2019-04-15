#include "app.h"
#include "user_main.h"
#include "string.h"
#include "stdlib.h"
typedef struct ble_device_info
{
	char buff[100];
	int len;
	int nowRevCount;
	int proRevCount;
	int aim;
}stuBledeviceInfo;


typedef struct ble_fifo_device
{
	char *buff;
	int readIndex;
	int writeIndex;
	int count;
	int curCount;
}stuBleFifo;
static stuBleFifo _stuBleFifoobj;
static stuBledeviceInfo stubleDI;
#if 1
/*********************************************************/
void createBleFifo(int count)
{
	if(_stuBleFifoobj.count != 0)
		cleanFifo();
	_stuBleFifoobj.count = count;
	_stuBleFifoobj.buff = (char *)malloc(count);
	_stuBleFifoobj.writeIndex = 0;
	_stuBleFifoobj.readIndex = 0;
	_stuBleFifoobj.curCount = 0;
}
	
void inputInfo(char *buff,int len)
{
	assert_param(_stuBleFifoobj.count > 0);
	//memcpy(_stuBleFifoobj.buff+,buff,len);
	for(int i = 0 ; i < len ; i ++)
	{
		_stuBleFifoobj.buff[_stuBleFifoobj.writeIndex] = buff[i];
		_stuBleFifoobj.writeIndex ++;
		_stuBleFifoobj.writeIndex = _stuBleFifoobj.writeIndex % _stuBleFifoobj.count;			
	}
	_stuBleFifoobj.curCount += len;
}

int getFifoData(char *ch)
{
	if(_stuBleFifoobj.readIndex == _stuBleFifoobj.writeIndex)
		return 0;
	*ch = _stuBleFifoobj.buff[_stuBleFifoobj.readIndex];
	_stuBleFifoobj.readIndex ++;
	_stuBleFifoobj.readIndex %= _stuBleFifoobj.count;
	_stuBleFifoobj.curCount --;
	return 1;
}
int getFIfoCount()
{
	return _stuBleFifoobj.curCount;
}
void cleanFifo(void)
{
	free(_stuBleFifoobj.buff);
	_stuBleFifoobj.count = 0;
	_stuBleFifoobj.writeIndex = 0;
	_stuBleFifoobj.readIndex = 0;
}
/*********************************************************/
#endif

void inputRevDebug(stuBledeviceInfo stubleDI)
{
	if(strstr(stubleDI.buff,"revCount") != NULL)
	{
		stubleDI.nowRevCount = 0;
		sscanf(stubleDI.buff,"revCount:%d;",&(stubleDI.proRevCount));
		printf("revcount = %d\r\n",stubleDI.proRevCount);
		createBleFifo(stubleDI.proRevCount);
	}
	else if(strstr(stubleDI.buff,"aim"))
	{
		sscanf(stubleDI.buff,"aim:%d;",&(stubleDI.aim));	
		printf("aim = %d\r\n",stubleDI.aim);		
	}
	else if(strstr(stubleDI.buff,"stop"))
	{
		stubleDI.nowRevCount = 0;
		stubleDI.proRevCount = 0;
		cleanFifo();
	}
	else			
	{
		//rev data
		//set a fifo		
		stubleDI.nowRevCount += stubleDI.len;
		inputInfo(stubleDI.buff,stubleDI.len);
	}
	
}

void _main(void)
{
	char ch;
	//init ble
	
	while(1)
	{
		if(getFIfoCount() > 0)
		{
			//send data to ble
			
		}
		HAL_Delay(10);	
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int count = 0;
	if(count++ == 100)
	{
		//printf("dog\r\n");
		count = 0;
	}
	getDebugData(stubleDI.buff,&(stubleDI.len));
	if(stubleDI.len > 0)
	{
		//ansy rev data
		inputRevDebug(stubleDI);
	}
}










