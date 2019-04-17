#ifndef BLEOPT
#define BLEOPT
#include "main.h"





extern void 			inputRevBle(void);
extern ErrorStatus 		getBleData(char *buff,int *len);
extern void 			checkBleData(void);


#define BLE_MODULE_MODE_CMD			1
#define BLE_REV_MODE_CMD			2
#define BLE_REV_MODE_DATA			3




#endif


