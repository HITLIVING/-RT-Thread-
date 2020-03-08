#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "lcd_app.h"
#include "drv_ili9341_lcd.h"

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


/*读取屏幕ID*/
static void lcd_ID(void)
{
	rt_kprintf("LCD ID is %d\n",ILI9341_Read_ID());
}
MSH_CMD_EXPORT(lcd_ID, LCD ID Check);

/*绘制直线*/
static void lcd_line(int argc, char *argv[])
{	
	ILI9341_DrawLine (atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
}
MSH_CMD_EXPORT(lcd_line, paint a line on the LCD <point1_x|point1_y|point2_x|point2_y>);

/*绘制矩形*/
static void lcd_Rectangle(int argc, char *argv[])
{	
	ILI9341_DrawRectangle (atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
}
MSH_CMD_EXPORT(lcd_Rectangle, paint a Rectangle on the LCD <point_x|point_y|width|height|full?>);

/*绘制圆形*/
static void lcd_Circle(int argc, char *argv[])
{	
	ILI9341_DrawCircle ( atoi(argv[1]) , atoi(argv[2]) , atoi(argv[3]) , atoi(argv[4]));
}
MSH_CMD_EXPORT(lcd_Circle, paint a Circle on the LCD <usX_Center|usY_Center|usRadius|full?>);

