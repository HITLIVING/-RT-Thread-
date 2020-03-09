#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "drv_beep.h"

void beep_init(void)
{
	rt_pin_mode(BEEP_DEV, PIN_MODE_OUTPUT);	
}

void beep_on_ms(rt_int32_t time)
{
	BEEP_SET;
	rt_thread_mdelay(time);
	BEEP_CLR;
}

















