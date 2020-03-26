/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "interface_app.h"
#include "schedule_app.h"
#include "bitmap_app.h"

#include "drv_flash.h"
#include "drv_ili9341_lcd.h"
#include "drv_xpt2049_lcd.h"
#include "drv_beep.h"
#include "drv_led.h"
#include "drv_key.h"
#include "drv_timer.h"

rt_bool_t main(void)
{	
	/********************Drive Init********************/
	/* Flash elm-Fats init */
	rt_hw_spi_flash_init();
	/* LCD drive init */
	ILI9341_Init ();
	/* Touch screen drive init */
	XPT2046_Init ();
	/* Beep drive init */
	beep_init();
	/* Led drive init */
	LED_Init();
	/* Key drive init */
	KEY_Init();

	/********************Application Init********************/
	/* System boot screen init */
	interface_system_Init();	
	/* System schedule init */
	thread_System_Schedule_init();
//	/* timer6 init */
//	timer6_init();

	
	return RT_EOK;
}






