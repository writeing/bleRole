#include "bleSend.h"
#include "main.h"
#include "usart.h"
#include "string.h"
/*************
func name:
func info: 
func parm; 
func return: 
*************/


/*************
func name:bleSendHeadInfo
func info: send stu buff with ble to device
func parm; stu ble info 
func return: void
*************/
void bleSendHeadInfoDevice(stuBledeviceInfo *temp)
{
	//check ble connect
	
	//combine stu char *
	char buff[50] = {0};
	int len = 0;
	
	memset(buff,0,50);
	len = sprintf(buff,"fileName:%s;",temp->filename);
	sendBuffToBleModule(buff,len);
	HAL_Delay(200);
	
	memset(buff,0,50);
	len = sprintf(buff,"fileSize:%d;",temp->filesize);
	sendBuffToBleModule(buff,len);
	HAL_Delay(200);
	
	memset(buff,0,50);
	len = sprintf(buff,"fileType:%s;",temp->filetype);
	sendBuffToBleModule(buff,len);
	HAL_Delay(200);
	
	memset(buff,0,50);
	len = sprintf(buff,"fileNum:%d;",temp->localNum);
	sendBuffToBleModule(buff,len);
	HAL_Delay(200);
	
	memset(buff,0,50);
	len = sprintf(buff,"fileEncrypt:%d;",temp->fileEncrypt);
	sendBuffToBleModule(buff,len);
	HAL_Delay(200);
	
	memset(buff,0,50);
	len = sprintf(buff,"FileObj:%d;",temp->_fileaim);
	sendBuffToBleModule(buff,len);
	HAL_Delay(200);
	//foreach send data
}
/*************
func name:serialSendHeadInfoPc
func info: send stu buff to pc with serial
func parm; stu ble info
func return: void
*************/
void serialSendHeadInfoPc(stuBledeviceInfo *temp)
{
	//combine rev head info 
	//send rev head info to pc
}

/*************
func name:serialSendBuffPc
func info: send buff to pc that rev buff for ble
func parm; buff,len
func return: send len
*************/
int serialSendBuffPc(char *buff,int len)
{
	int sendLen = 0;
	//check buff is null,check len <= 0
	//send buff to pc 
	//return send len oradd
	return sendLen;
}

/*************
func name:bleSendBuffDevice
func info: send buff to device that rev buff for pc
func parm; buff,len
func return: send len
*************/
int bleSendBuffDevice(char *buff,int len)
{
	int sendLen = 0;
	//check buff is null,check len <= 0
	//send buff to ble
	//return send len oradd
	return sendLen;
}


/*************
func name:sendBuffToBleModule
func info: send buff to ble AT or data
func parm; 
func return: void
*************/
void sendBuffToBleModule(char *buff ,int len)
{
	HAL_UART_Transmit(&huart2,(uint8_t *)buff,len,0xff);
}












