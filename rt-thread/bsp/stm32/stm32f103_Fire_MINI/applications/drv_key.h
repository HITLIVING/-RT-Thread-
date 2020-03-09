#ifndef __DRV_KEY_H__
#define __DRV_KEY_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define KEY1_DEV GET_PIN(A, 0)	
#define KEY2_DEV GET_PIN(C, 13)	
	
	
void KEY_Init(void);



#endif



