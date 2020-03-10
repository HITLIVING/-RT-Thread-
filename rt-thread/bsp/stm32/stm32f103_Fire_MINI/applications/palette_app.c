#include "palette_app.h"

#include "drv_xpt2049_lcd.h"
#include "drv_ili9341_lcd.h"

/**
* @brief  Palette_Init �����ʼ��
* @param  ��
* @retval ��
*/
void Palette_Init(uint8_t LCD_Mode)
{	
    /* Clean all screen */
    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	

	
}

/**
* @brief  Draw_Trail �ڻ���������津���켣
* @param  pre_x ��һ���x����
* @param  pre_y ��һ���y����
* @param  x     ����һ���x����
* @param  y     ����һ���y����
* @param  brush ��ˢ����
* @retval ��
*/
void Draw_Trail(int16_t pre_x,int16_t pre_y,int16_t x,int16_t y)
{                 
   if(pre_x< 0 || pre_y < 0) //�µıʼ�
   {      
		ILI9341_SetPointPixel(x,y);              
   }
   else //������һ�εıʼ�
   {      
        ILI9341_DrawLine(pre_x,pre_y,x,y);
   }                 
}



/* ------------------------------------------end of file---------------------------------------- */




























