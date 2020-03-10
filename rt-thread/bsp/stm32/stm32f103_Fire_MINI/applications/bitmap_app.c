#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "bitmap_app.h"
#include "drv_ili9341_lcd.h"

void bitmap_RightArr_build(rt_uint32_t Point_x,rt_uint32_t Point_y)
{
	rt_uint16_t point[8][4]={{1,7,1,9},	{2,7,2,9},	{3,7,3,9},{4,4,4,12},
							 {5,5,5,11},{6,6,6,10},	{7,7,7,9},{8,8,8,8}};
	
	for(rt_uint8_t i=0;i<=7;i++)
	{
		ILI9341_DrawLine (	Point_x-1+point[i][0], Point_y+1+point[i][1], 
							Point_x-1+point[i][2], Point_y+1+point[i][3]);
	}

}

void bitmap_RightArr_clear(rt_uint32_t Point_x,rt_uint32_t Point_y)
{
	ILI9341_Clear (Point_x-1+1, Point_y+1+4, 8, 9);
}


