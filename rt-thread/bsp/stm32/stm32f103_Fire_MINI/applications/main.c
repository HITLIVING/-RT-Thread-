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
#include "ili9341_lcd.h"

int main(void)
{	
	rt_hw_spi_flash_init();
	ILI9341_Init ();
}


