#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "interface_app.h"
#include "message_app.h"
#include "drv_ili9341_lcd.h"
#include "drv_xpt2049_lcd.h"
#include "drv_beep.h"

//*************************˽�к���***********************************


char *time_string;
void interface_time_show()
{
	/* ��ʾʱ�� */	       
    time_t now = time(RT_NULL);
    time_string = ctime(&now);
	time_string[20] = '\0';
	time_string[10] = ' ';
	time_string+=10;
	LCD_SetFont(&Font24x32);
	ILI9341_DispStringLine_EN (LINE(4),time_string);
	LCD_SetFont(&Font8x16);
}

void interface_system_Init(void)
{
	ILI9341_Clear (0, 0, 240, 320);	
	//LCD��ʼ�������
	LCD_SetFont(&Font24x32);	
	ILI9341_DispStringLine_EN (LINE(4),  "RT-Thread!");
	LCD_SetFont(&Font8x16);
	ILI9341_DispStringLine_EN (LINE(19),  "V4.0.3 Operate System");
	rt_thread_mdelay(1000);
	interface_system_reset();	
}

void interface_system_reset(void)
{
	//��������
	LCD_SetColors(BLUE_SPE, BLACK);
	ILI9341_Clear (0, 0, 240, 320);
	LCD_SetFont(&Font8x16);
	message_SysReaday();
}


//***************************�����п���*******************************


/*��ӡһ���ַ�*/
static void lcd_Printf(int argc, char *argv[])
{	
	ILI9341_DispStringLine_EN (  LINE(atoi(argv[1])),  argv[2] );
}
MSH_CMD_EXPORT(lcd_Printf, print string on LCD in setting line <line No.|words>);

/*���д�����У׼*/
static void touch_Check(int argc, char *argv[])
{	
	rt_kprintf("LCD Touch Checking with Mode %d\n",6);
	XPT2046_Touch_Calibrate(6);	
	interface_system_reset();
}
MSH_CMD_EXPORT(touch_Check, check the LCD touch function <mode No.>);




