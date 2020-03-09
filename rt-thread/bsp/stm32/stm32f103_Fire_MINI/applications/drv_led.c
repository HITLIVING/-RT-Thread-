#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "drv_led.h"

void LED_Init(void)
{
	//����GPIO����ģʽ
	rt_pin_mode( LED1_DEV, PIN_MODE_OUTPUT );
	rt_pin_mode( LED2_DEV, PIN_MODE_OUTPUT );
	
	LED1_SET;
	LED2_SET;
}

