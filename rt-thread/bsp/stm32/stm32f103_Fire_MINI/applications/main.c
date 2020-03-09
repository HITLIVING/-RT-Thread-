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

#include "flash_app.h"
#include "drv_ili9341_lcd.h"
#include "drv_xpt2049_lcd.h"

int main(void)
{	
	/* Flash芯片文件系统初始化 */
	rt_hw_spi_flash_init();
	/* LCD显示屏初始化 */
	ILI9341_Init ();
	/* 触摸板初始化 */
	XPT2046_Init ();
	
	
}


