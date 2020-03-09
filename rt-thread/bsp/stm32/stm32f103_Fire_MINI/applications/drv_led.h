#ifndef __DRV_LED_H__
#define __DRV_LED_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED1_DEV GET_PIN(C, 2)	//��ȡC2���ź�
#define LED2_DEV GET_PIN(C, 3)	//��ȡC3���ź�

#define	LED1_SET rt_pin_write(LED1_DEV, PIN_HIGH)
#define	LED1_CLR rt_pin_write(LED1_DEV, PIN_LOW)

#define	LED2_SET rt_pin_write(LED2_DEV, PIN_HIGH)
#define	LED2_CLR rt_pin_write(LED2_DEV, PIN_LOW)	
	
	
void LED_Init(void);



#endif



