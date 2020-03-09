#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "lcd_app.h"
#include "drv_ili9341_lcd.h"
#include "drv_xpt2049_lcd.h"


//*************************私有函数***********************************
static void lcd_main_time(void);

char *time_string;
static void lcd_main_time()
{
	/* 显示时间 */	       
    time_t now = time(RT_NULL);
    time_string = ctime(&now);
	time_string[20] = '\0';
	time_string[10] = ' ';
	time_string+=10;
	LCD_SetFont(&Font24x32);
	ILI9341_DispStringLine_EN (LINE(4),time_string);
	LCD_SetFont(&Font8x16);
}

//*************************调用函数***********************************

void lcd_system_Init(void)
{
	ILI9341_Clear (0, 0, 240, 320);	
	//LCD初始界面测试
	LCD_SetFont(&Font24x32);	
	ILI9341_DispStringLine_EN (LINE(4),  "RT-Thread!");
	LCD_SetFont(&Font8x16);
	ILI9341_DispStringLine_EN (LINE(19),  "V4.0.3 Operate System");
	rt_thread_mdelay(1000);
	lcd_system_reset();	
}

void lcd_system_reset(void)
{
	//重置设置
	LCD_SetColors(BLUE_SPE, BLACK);
	ILI9341_Clear (0, 0, 240, 320);
	LCD_SetFont(&Font8x16);
	ILI9341_DispStringLine_EN (LINE(19),  "[Mes] System is readey");
}

//****************************Main UI thread***********************************
#define MAIN_THREAD_PRIORITY         25
#define MAIN_THREAD_STACK_SIZE       1024
#define MAIN_THREAD_TIMESLICE        100

/* 线程 1 的入口函数 */
static void thread_main_entry(void *parameter)
{		
	while(1)
	{
		rt_thread_mdelay(1000);	
		lcd_main_time();		
	}		
}

ALIGN(RT_ALIGN_SIZE)

void thread_main_init(void)
{
	rt_thread_t thread_main_obj = RT_NULL;
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
    thread_main_obj = rt_thread_create("thread_main_LCD",
                            thread_main_entry, RT_NULL,
                            MAIN_THREAD_STACK_SIZE,
                            MAIN_THREAD_PRIORITY, MAIN_THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (thread_main_obj != RT_NULL)
	{
		rt_thread_startup(thread_main_obj);
	}      
	else
	{
		LCD_ClearLine(LINE(19));
		ILI9341_DispStringLine_EN (LINE(19),  "[Error] Build main UI failed!");		
	}
}

//***************************命令行控制*******************************

/*设置背景和字体颜色*/
static rt_err_t lcd_setcolors(int argc, char *argv[])
{		
	if(!rt_strcmp(argv[1],"WHITE"))
	{LCD_SetBackColor(WHITE);}
	else if(!rt_strcmp(argv[1],"BLACK"))         
	{LCD_SetBackColor(BLACK);}
	else if(!rt_strcmp(argv[1],"GREY"))         
	{LCD_SetBackColor(GREY);}                
	else if(!rt_strcmp(argv[1],"BLUE"))      
	{LCD_SetBackColor(BLUE);}              
	else if(!rt_strcmp(argv[1],"BLUE2"))   
	{LCD_SetBackColor(BLUE2);}             
	else if(!rt_strcmp(argv[1],"RED"))     
	{LCD_SetBackColor(RED);}               
	else if(!rt_strcmp(argv[1],"MAGENTA")) 
	{LCD_SetBackColor(MAGENTA);}                
	else if(!rt_strcmp(argv[1],"GREEN"))      
	{LCD_SetBackColor(GREEN);}
	else if(!rt_strcmp(argv[1],"CYAN"))      
	{LCD_SetBackColor(CYAN);}	
	else if(!rt_strcmp(argv[1],"YELLOW"))
	{LCD_SetBackColor(YELLOW);}
	else if(!rt_strcmp(argv[1],"BRED")) 
	{LCD_SetBackColor(BRED);}                
	else if(!rt_strcmp(argv[1],"GRED"))      
	{LCD_SetBackColor(GRED);}                
	else if(!rt_strcmp(argv[1],"GBLUE"))
	{LCD_SetBackColor(GBLUE);}
	else
	{
		rt_kprintf("changed failed\n");
		return RT_ERROR;
	}
	
	if(!rt_strcmp(argv[2],"BLACK"))
	{LCD_SetTextColor(BLACK);}
	else if(!rt_strcmp(argv[2],"WHITE"))         
	{LCD_SetTextColor(WHITE);}
	else if(!rt_strcmp(argv[2],"CYAN"))         
	{LCD_SetTextColor(CYAN);}
	else if(!rt_strcmp(argv[2],"GREY"))         
	{LCD_SetTextColor(GREY);}                
	else if(!rt_strcmp(argv[2],"BLUE"))      
	{LCD_SetTextColor(BLUE);}              
	else if(!rt_strcmp(argv[2],"BLUE2"))   
	{LCD_SetTextColor(BLUE2);}             
	else if(!rt_strcmp(argv[2],"RED"))     
	{LCD_SetTextColor(RED);}               
	else if(!rt_strcmp(argv[2],"MAGENTA")) 
	{LCD_SetTextColor(MAGENTA);}                
	else if(!rt_strcmp(argv[2],"GREEN"))      
	{LCD_SetTextColor(GREEN);}                
	else if(!rt_strcmp(argv[2],"YELLOW"))
	{LCD_SetTextColor(YELLOW);}
	else if(!rt_strcmp(argv[2],"BRED")) 
	{LCD_SetTextColor(BRED);}                
	else if(!rt_strcmp(argv[2],"GRED"))      
	{LCD_SetTextColor(GRED);}                
	else if(!rt_strcmp(argv[2],"GBLUE"))
	{LCD_SetTextColor(GBLUE);}
	else
	{
		rt_kprintf("changed failed\n");
		return RT_ERROR;
	}	

    rt_kprintf("changed seccessfully\n");
	ILI9341_Clear (0, 0, 240, 320);
	
	return RT_EOK;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(lcd_setcolors, LCD change color: <Background|Text>);


/*设置字符大小*/
static void lcd_Wordsize(int argc, char *argv[])
{	
	if(atoi(argv[1])==3)
	{
		LCD_SetFont(&Font24x32);
	}
	else if(atoi(argv[1])==2)
	{
		LCD_SetFont(&Font16x24);
	}
	else
	{
		LCD_SetFont(&Font8x16);		
	}
	
	rt_kprintf("Watch is  %d\n",rt_pin_read(XPT2046_PENIRQ));
	
}
MSH_CMD_EXPORT(lcd_Wordsize, set the size of words on LCD <size(1,2,3)>);

/*打印一串字符*/
static void lcd_Printf(int argc, char *argv[])
{	
	ILI9341_DispStringLine_EN (  LINE(atoi(argv[1])),  argv[2] );
}
MSH_CMD_EXPORT(lcd_Printf, print string on LCD in setting line <line No.|words>);

/*运行触摸屏校准*/
static void touch_Check(int argc, char *argv[])
{	
	rt_uint32_t Mode_Num = atoi(argv[1]);
	
	if(Mode_Num>=1&&Mode_Num<=6)
	{
		XPT2046_Touch_Calibrate(Mode_Num);
		rt_kprintf("LCD Touch Checking with Mode %d\n",Mode_Num);
	}
	else
	{
		XPT2046_Touch_Calibrate(6);
		rt_kprintf("LCD Touch Checking with Mode 6\n");
	}
	
	lcd_system_reset();
}
MSH_CMD_EXPORT(touch_Check, check the LCD touch function <mode No.>);




