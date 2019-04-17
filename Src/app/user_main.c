#include "usart.h"
#include "user_main.h"
#include "string.h"
#include "FIFO_Uart.h"
#include "app.h"
#include "bleOpt.h"
#include "fifo.h"
int revBuffSize = 1024;
char revBuff[128];

UART_FIFO_Typedef_t usart_debug;
UART_FIFO_Typedef_t usart_ble;

uint8_t rx_debug_buff[1024] = {0};
uint8_t tx_debug_buff[1024] = {0};

uint8_t rx_ble_buff[1024] = {0};
uint8_t tx_ble_buff[1024] = {0};

static uint8_t bleCh;
static uint8_t debugCh;

extern stuBledeviceInfo stubleDI;
extern stuBledeviceInfo stuDebugDI;

extern int 				revDataMode;
extern int 				revBleDataMode;
extern stuBleFifo 		blestuBleFifoobj;
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xff);
	return 1;
}
void sendBuffToPc(uint8_t *buff,int len)
{
	HAL_UART_Transmit(&huart1,buff,len,0xff);
}
void inputFifo(char ch,UART_FIFO_Typedef_t *usart)
{
	if((usart)->pRxBuf != NULL)
	{
		usart->pRxBuf[usart->usRxWrite] = ch;
		if (++(usart)->usRxWrite >= (usart)->usRxBufSize)
		{
			(usart)->usRxWrite = 0;
		}
		if ((usart)->usRxCount < (usart)->usRxBufSize)
		{
			(usart)->usRxCount++;
		}
		else
		{
			(usart)->usRxRead = (usart)->usRxWrite;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		//huart1 rev data
		//printf("%c\r\n",debugCh);
		if(revDataMode == DEBUG_REV_MODE_DATA)
		{
			inputCharInfo(&_stuBleFifoobj,debugCh);
		}
		else
		{
			inputFifo(debugCh,&usart_debug);
		}
		HAL_UART_Receive_IT(huart,&debugCh,1);
	}
	if(huart == &huart2)
	{
		//huart1 rev data
		//printf("%c\r\n",debugCh);		
		if(revBleDataMode == BLE_REV_MODE_DATA)
		{
			inputCharInfo(&blestuBleFifoobj,bleCh);
		}
		else
		{
			inputFifo(bleCh,&usart_ble);
		}
		HAL_UART_Receive_IT(huart,&bleCh,1);
	}
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	printf("rev error\r\n");
	uint32_t isrflags   = READ_REG(huart->Instance->SR);//?????,???????SR
	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_PE))!=RESET)
	{
			READ_REG(huart->Instance->DR);//PE???,????DR
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);//???
	}
	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_FE))!=RESET)
	{
			READ_REG(huart->Instance->DR);//FE???,????DR
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
	}

	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_NE))!=RESET)
	{
			READ_REG(huart->Instance->DR);//NE???,????DR
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
	}        

	if((__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE))!=RESET)
	{
			READ_REG(huart->Instance->CR1);//ORE???,????CR
			__HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
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


ErrorStatus getDebugData(char *buff,int *len)
{
	uint8_t aRxBuffer;
	*len = 0;
	while(FIFO_UartReadByte(&usart_debug,&aRxBuffer) == HAL_OK)	
	{
		buff[*len] = aRxBuffer;		
		(*len) ++;
	}
	return SUCCESS;
}

void initFifo(void)
{
	//FIFO_UartVarInit(&usart_debug,&huart1,tx_debug_buff,rx_debug_buff,1024,1024,NULL,NULL,NULL);
	
	HAL_UART_Receive_DMA(&huart1,(uint8_t *)_stuBleFifoobj.buff, 1024);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	
	//FIFO_UartVarInit(&usart_ble,&huart2,tx_ble_buff,rx_ble_buff,1024,1024,NULL,NULL,NULL);
	
	HAL_UART_Receive_DMA(&huart2,(uint8_t *)blestuBleFifoobj.buff, 1024);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int count = 0;
	if(count++ == 100)
	{
		//printf("dog\r\n");
		count = 0;
	}
	//ErrorStatus rpy = getDebugData(,&(stuDebugDI.len));
	if(blestuBleFifoobj.revFlag == 1)
	{
		blestuBleFifoobj.revFlag = 0;
		memcpy(stubleDI.buff,blestuBleFifoobj.buff,blestuBleFifoobj.curCount);
		stubleDI.len = blestuBleFifoobj.curCount;
		if(stubleDI.len > 0)
		{
			//ansy rev data		
			inputRevBle();
		}
	}
	if(_stuBleFifoobj.revFlag == 1)
	{
		_stuBleFifoobj.revFlag = 0;
		memcpy(stuDebugDI.buff,_stuBleFifoobj.buff,_stuBleFifoobj.curCount);
		stuDebugDI.len = _stuBleFifoobj.curCount;
		if(stuDebugDI.len > 0)
		{
			//ansy rev data		
			inputRevDebug();
		}
	}
}


void UsartReceive_IDLE(UART_HandleTypeDef *huart,stuBleFifo *stutemp)
{
	uint32_t temp;
 
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		HAL_UART_DMAStop(huart);
		temp = huart->hdmarx->Instance->CNDTR;
		stutemp->curCount=  1024 - temp; 
		stutemp->revFlag = 1;
		HAL_UART_Receive_DMA(huart,(uint8_t *)stutemp->buff,1024);
	}
}


ErrorStatus checkRevData(int num)
{
	int num1 = num/10;
	int num2 = num%10;
	if(stuDebugDI.localNum == num1 || stuDebugDI.localNum == num2)
	{
		return SUCCESS;		
	}
	return ERROR;
}

