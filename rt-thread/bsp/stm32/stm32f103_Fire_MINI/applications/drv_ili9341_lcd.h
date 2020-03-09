#ifndef      __ILI9341_LCD_H
#define	     __ILI9341_LCD_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "drv_fonts.h"
#include "stm32f1xx_hal.h"

/******控制信号线******/
//片选
#define ILI9341_CS GET_PIN(C, 4)
//数据控制切换
#define ILI9341_DC GET_PIN(C, 7)
//写使能
#define ILI9341_WR GET_PIN(C, 6)
//读使能
#define ILI9341_RD GET_PIN(C, 5)
//背光
#define ILI9341_BK GET_PIN(D, 2)

////控制线写入
#define	ILI9341_CS_SET rt_pin_write(ILI9341_CS, PIN_HIGH);
#define	ILI9341_DC_SET rt_pin_write(ILI9341_DC, PIN_HIGH);
#define	ILI9341_WR_SET rt_pin_write(ILI9341_WR, PIN_HIGH);
#define	ILI9341_RD_SET rt_pin_write(ILI9341_RD, PIN_HIGH);
#define	ILI9341_BK_SET rt_pin_write(ILI9341_BK, PIN_HIGH);
//						
#define	ILI9341_CS_CLR rt_pin_write(ILI9341_CS, PIN_LOW);
#define	ILI9341_DC_CLR rt_pin_write(ILI9341_DC, PIN_LOW);
#define	ILI9341_WR_CLR rt_pin_write(ILI9341_WR, PIN_LOW);
#define	ILI9341_RD_CLR rt_pin_write(ILI9341_RD, PIN_LOW);
#define	ILI9341_BK_CLR rt_pin_write(ILI9341_BK, PIN_LOW);

/********数据信号线***************/
#define ILI9341_D0 GET_PIN(B, 0)
#define ILI9341_D1 GET_PIN(B, 1)
#define ILI9341_D2 GET_PIN(B, 2)
#define ILI9341_D3 GET_PIN(B, 3)
#define ILI9341_D4 GET_PIN(B, 4)
#define ILI9341_D5 GET_PIN(B, 5)
#define ILI9341_D6 GET_PIN(B, 6)
#define ILI9341_D7 GET_PIN(B, 7)
#define ILI9341_D8 GET_PIN(B, 8)
#define ILI9341_D9 GET_PIN(B, 9)
#define ILI9341_D10 GET_PIN(B, 10)
#define ILI9341_D11 GET_PIN(B, 11)
#define ILI9341_D12 GET_PIN(B, 12)
#define ILI9341_D13 GET_PIN(B, 13)
#define ILI9341_D14 GET_PIN(B, 14)
#define ILI9341_D15 GET_PIN(B, 15)
#define ILI9341_DATA_PORT GPIOB

//数据线输入输出
#define DATAOUT(x) 	ILI9341_DATA_PORT->ODR=x; //数据输出
#define DATAIN     	ILI9341_DATA_PORT->IDR;   //数据输入

/*************************************** 调试延时预用 ******************************************/
#define DEBUG_DELAY() ILI9341_Delay(0xFFFF)         

/***************************** ILI934 显示区域的起始坐标和总行列数 ***************************/
#define      ILI9341_DispWindow_X_Star		    0     //起始点的X坐标
#define      ILI9341_DispWindow_Y_Star		    0     //起始点的Y坐标


#define ILI9341_LESS_PIXEL 240			//液晶屏较短方向的像素宽度
#define ILI9341_MORE_PIXEL 320			//液晶屏较长方向的像素宽度

//根据液晶扫描方向而变化的XY像素宽度，调用ILI9341_GramScan函数设置方向时会自动更改
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

//液晶屏扫描模式选值为0-7
extern uint8_t LCD_SCAN_MODE;

/******************************* 定义 ILI934 显示屏常用颜色 ********************************/
#define      WHITE		0xFFFF	   //白色
#define      BLACK      0x0000	   //黑色 
#define      GREY       0xF7DE	   //灰色 
#define      BLUE       0x001F	   //蓝色 
#define      BLUE2      0x051F	   //浅蓝色
#define		 BLUE_SPE	0x07FF	   //特殊蓝
#define      RED        0xF800	   //红色 
#define      MAGENTA    0xF81F	   //红紫色，洋红色 
#define      GREEN      0x07E0	   //绿色 
#define      CYAN       0x7FFF	   //蓝绿色，青色 
#define      YELLOW     0xFFE0	   //黄色 
#define      BRED       0xF81F	   //紫色
#define      GRED       0xFFE0	   //蛋黄
#define      GBLUE      0x07FF	   //天蓝色

/******************************* 定义 ILI934 常用命令 ********************************/
#define      CMD_SetCoordinateX		 		    0x2A	     //设置X坐标
#define      CMD_SetCoordinateY		 		    0x2B	     //设置Y坐标
#define      CMD_SetPixel		 		        0x2C	     //填充像素

void ILI9341_Init ( void );
void LCD_SetTextColor(uint16_t Color);
void LCD_SetBackColor(uint16_t Color);
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor); 
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor);

void ILI9341_GramScan ( uint8_t ucOption );
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void ILI9341_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void ILI9341_SetPointPixel ( uint16_t usX, uint16_t usY );	

void ILI9341_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 );
void ILI9341_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled );
void ILI9341_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled );

uint16_t ILI9341_GetPointPixel ( uint16_t usX, uint16_t usY );
uint16_t ILI9341_Read_ID(void);

void ILI9341_DispStringLine_EN (  uint16_t line,  char * pStr );
void ILI9341_DispString_EN ( 	uint16_t usX ,uint16_t usY,  char * pStr );
void ILI9341_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr );
void LCD_SetFont(sFONT *fonts);
void LCD_ClearLine(uint16_t Line);

sFONT *LCD_GetFont(void);


//C99语法下内联函数的特殊使用

extern __inline void                 ILI9341_Write_Cmd           ( uint16_t usCmd );
extern __inline void                 ILI9341_Write_Data          ( uint16_t usData );
extern __inline uint16_t             ILI9341_Read_Data           ( void );
extern __inline void          ILI9341_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );


/**
  * @brief  向ILI9341写入命令
  * @param  usCmd :要写入的命令（表寄存器地址）
  * @retval 无
  */	
__inline void ILI9341_Write_Cmd ( uint16_t usCmd )
{
	ILI9341_CS_CLR;//开始片选      
	ILI9341_DC_CLR;//写命令
	ILI9341_RD_SET;//禁止读
	DATAOUT(usCmd);//输出命令
	ILI9341_WR_CLR;//写入开始
	ILI9341_WR_SET;//写入结束
	ILI9341_CS_SET;//结束片选        	
}


/**
  * @brief  向ILI9341写入数据
  * @param  usData :要写入的数据
  * @retval 无
  */	
__inline void ILI9341_Write_Data ( uint16_t usData )
{
	ILI9341_CS_CLR;//开始片选      
	ILI9341_DC_SET;//写数据
	ILI9341_RD_SET;//禁止读
	DATAOUT(usData);//输出数据
	ILI9341_WR_CLR;//写入开始
	ILI9341_WR_SET;//写入结束
	ILI9341_CS_SET;//结束片选   
	
}


/**
  * @brief  从ILI9341读取数据
  * @param  无
  * @retval 读取到的数据
  */	
__inline uint16_t ILI9341_Read_Data ( void )
{
	uint16_t data;
 	ILI9341_DATA_PORT->CRL=0X88888888; //上拉输入
	ILI9341_DATA_PORT->CRH=0X88888888; //上拉输入
	ILI9341_DATA_PORT->ODR=0X0000;     //全部输出0

	ILI9341_DC_SET;
	ILI9341_WR_SET;

	ILI9341_CS_CLR;
	//读取数据
	ILI9341_RD_CLR;    
    
	data=DATAIN;  
	ILI9341_RD_SET;
	ILI9341_CS_SET; 

	ILI9341_DATA_PORT->CRL=0X33333333; // 上拉输出
	ILI9341_DATA_PORT->CRH=0X33333333; // 上拉输出
	ILI9341_DATA_PORT->ODR=0XFFFF;    //全部输出高
	return data;  
}

/**
 * @brief  在ILI9341显示器上以某一颜色填充像素点
 * @param  ulAmout_Point ：要填充颜色的像素点的总数目
 * @param  usColor ：颜色
 * @retval 无
 */
extern __inline void ILI9341_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	
	/* memory write */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
		ILI9341_Write_Data ( usColor );	
}

#endif /* __ILI9341_ILI9341_H */


