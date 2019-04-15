#include "usart.h"
#include "user_main.h"
#include "string.h"
#include "FIFO_Uart.h"

int revBuffSize = 1024;
char revBuff[128];

UART_FIFO_Typedef_t usart_debug;
UART_FIFO_Typedef_t usart_ble;

uint8_t rx_debug_buff[200] = {0};
uint8_t tx_debug_buff[200] = {0};

uint8_t rx_ble_buff[256] = {0};
uint8_t tx_ble_buff[128] = {0};

static uint8_t bleCh;
static uint8_t debugCh;


int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xff);
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
		inputFifo(debugCh,&usart_debug);
		if(HAL_UART_Receive_IT(huart,&debugCh,1) != HAL_OK)
		{
			if(HAL_UART_Init(huart) != HAL_OK)
			{
				printf("error set IT\r\n");
			}
			if(HAL_UART_Receive_IT(huart,&debugCh,1) == HAL_ERROR)
			{
				HAL_NVIC_SystemReset();
			}
		}
	}
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
	FIFO_UartVarInit(&usart_debug,&huart1,tx_debug_buff,rx_debug_buff,200,200,NULL,NULL,NULL);
	if(HAL_UART_Receive_IT(&huart1,&debugCh,1) != HAL_OK)
	{
		printf("huart1 init fasle ,reset\r\n");
	}
	FIFO_UartVarInit(&usart_ble,&huart2,tx_ble_buff,rx_ble_buff,256,128,NULL,NULL,NULL);
	if(HAL_UART_Receive_IT(&huart2,&bleCh,1) != HAL_OK)
	{
		printf("huart2 init fasle ,reset\r\n");
	}
}


