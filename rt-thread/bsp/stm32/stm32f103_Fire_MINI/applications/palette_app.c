#include "palette_app.h"

#include "drv_xpt2049_lcd.h"
#include "drv_ili9341_lcd.h"

/**
* @brief  Palette_Init 画板初始化
* @param  无
* @retval 无
*/
void Palette_Init(uint8_t LCD_Mode)
{	
    /* Clean all screen */
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	

	
}

/**
* @brief  Draw_Trail 在画板区域描绘触摸轨迹
* @param  pre_x 上一点的x坐标
* @param  pre_y 上一点的y坐标
* @param  x     最新一点的x坐标
* @param  y     最新一点的y坐标
* @param  brush 画刷参数
* @retval 无
*/
void Draw_Trail(int16_t pre_x,int16_t pre_y,int16_t x,int16_t y)
{                 
   if(pre_x< 0 || pre_y < 0) //新的笔迹
   {      
		ILI9341_SetPointPixel(x,y);              
   }
   else //继续上一次的笔迹
   {      
        ILI9341_DrawLine(pre_x,pre_y,x,y);
   }                 
}



/* ------------------------------------------end of file---------------------------------------- */




























