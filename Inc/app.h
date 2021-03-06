#ifndef APP
#define APP

extern void _main(void);

typedef struct ble_device_info
{
	char	buff[1025];
	int 	len;
	int 	nowRevCount;
	int		proRevCount;
	char 	filename[20];
	int 	filesize;
	char 	filetype[10];
	int 	localNum;
	int 	fileEncrypt;
	int 	fileObj[3];
	int 	aimNum;
	int 	_fileaim;
}stuBledeviceInfo;


typedef struct ble_fifo_device
{
	char 	buff[1025];
	int 	readIndex;
	int 	writeIndex;
	int 	count;
	int 	curCount;
	unsigned int g_timeFlag;
	int 	revFlag;
}stuBleFifo;


extern void 	inputRevDebug(void);

#define DEBUG_REV_MODE_CMD			1
#define DEBUG_REV_MODE_DATA			2


extern stuBleFifo 		_stuBleFifoobj;
extern stuBledeviceInfo stuDebugDI;




#endif
