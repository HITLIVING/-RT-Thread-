#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "menu_app.h"
#include "schedule_app.h"
#include "bitmap_app.h"

#include "drv_ili9341_lcd.h"

enum MainSchStateType  MainSchStep_choose = CheckTouch_State;
uint8_t menu_select_changedFlag = 0;

static char *time_string;
void menu_timeshow(void)
{
	/* ��ʾʱ�� */	       
    time_t now = time(RT_NULL);
    time_string = ctime(&now);
	time_string[20] = '\0';
	time_string+=11;
	ILI9341_DispStringLine_EN (LINE(0),time_string);
}

void menu_init(void)
{
	MainSchStep_choose = CheckTouch_State;
	ILI9341_DispStringLine_EN ( LINE(1),  "  --------System Menu-------" );
	ILI9341_DispStringLine_EN ( LINE(3),  "   Touch Screen Calibration" );
	ILI9341_DispStringLine_EN ( LINE(5),  "   Palette Tools" );
	ILI9341_DispStringLine_EN ( LINE(7),  "   Steering Control" );
	ILI9341_DispStringLine_EN ( LINE(9),  "   Gyroscope Date" );
	ILI9341_DispStringLine_EN ( LINE(11), "" );
	ILI9341_DispStringLine_EN ( LINE(13), "" );
	ILI9341_DispStringLine_EN ( LINE(15), "" );
	
	ILI9341_DispStringLine_EN ( LINE(17), "   Back" );
	
}

void menu_select(void)
{
	/* while  select has been changed Enter*/
	if(menu_select_changedFlag==1)
	{
		/* reset flag */
		menu_select_changedFlag=0;
		
		/* Clear last choose arrow */
		bitmap_RightArr_clear(8,16*(2*(MainSchStep_choose-1)-1));
		
		if(MainSchStep_choose==Type_Num)
		{
			/* jump arrow to the last line : back */
			bitmap_RightArr_build(8,16*17);
		}
		else
		{
			/* when the arrow back to the first line clear the last line arrow*/
			if(MainSchStep_choose==CheckTouch_State)
			{
				bitmap_RightArr_clear(8,16*17);
			}
			
			/* draw the arrow */
			bitmap_RightArr_build(8,16*(2*MainSchStep_choose-1));
		}	
	}
}









