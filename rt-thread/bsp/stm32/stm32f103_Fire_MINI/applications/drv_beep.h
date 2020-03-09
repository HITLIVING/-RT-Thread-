#ifndef __DRV_BEEP_H__
#define __DRV_BEEP_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define BEEP_DEV GET_PIN(C, 1)

#define	BEEP_SET rt_pin_write(BEEP_DEV, PIN_HIGH)
#define	BEEP_CLR rt_pin_write(BEEP_DEV, PIN_LOW)

void beep_init(void);
void beep_on_ms(rt_int32_t time);


#endif

