#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "message_app.h"
#include "drv_ili9341_lcd.h"
#include "drv_beep.h"

void message_SysReaday(void)
{
	LCD_SetFont(&Font8x16);
	LCD_ClearLine(LINE(19));
	beep_on_ms(100);
	ILI9341_DispStringLine_EN (LINE(19),  "[Mes] System is readey");
}

void error_SchFailed(void)
{
	LCD_SetFont(&Font8x16);
	LCD_ClearLine(LINE(19));
	beep_on_ms(100);
	ILI9341_DispStringLine_EN (LINE(19),  "[Error] Build Sys_Sch failed!");	
}

void message_toMenu(void)
{
	LCD_SetFont(&Font8x16);
	LCD_ClearLine(LINE(13));
	ILI9341_DispStringLine_EN (LINE(13),  " -Press K1 or K2 to Continue- ");
}

void message_Menu(void)
{
	LCD_SetFont(&Font8x16);
	LCD_ClearLine(LINE(19));
	beep_on_ms(100);
	ILI9341_DispStringLine_EN (LINE(19),  "Press K1 to elect,K2 to Enter");
}

void message_CheckPrepare(void)
{
	LCD_SetFont(&Font8x16);
	LCD_ClearLine(LINE(19));
	beep_on_ms(100);
	ILI9341_DispStringLine_EN (LINE(19),  "[Mes] TouchCheck is Prepared");
}

void message_BackToMenu(void)
{
	LCD_SetFont(&Font8x16);
	LCD_ClearLine(LINE(19));
	beep_on_ms(100);
	ILI9341_DispStringLine_EN (LINE(19),  "[Mes] Press K2 Back to Menu");
}

















