#ifndef      __ILI9341_LCD_H
#define	     __ILI9341_LCD_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "drv_fonts.h"
#include "stm32f1xx_hal.h"

/******�����ź���******/
//Ƭѡ
#define ILI9341_CS GET_PIN(C, 4)
//���ݿ����л�
#define ILI9341_DC GET_PIN(C, 7)
//дʹ��
#define ILI9341_WR GET_PIN(C, 6)
//��ʹ��
#define ILI9341_RD GET_PIN(C, 5)
//����
#define ILI9341_BK GET_PIN(D, 2)

////������д��
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

/********�����ź���***************/
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

//�������������
#define DATAOUT(x) 	ILI9341_DATA_PORT->ODR=x; //�������
#define DATAIN     	ILI9341_DATA_PORT->IDR;   //��������

/*************************************** ������ʱԤ�� ******************************************/
#define DEBUG_DELAY() ILI9341_Delay(0xFFFF)         

/***************************** ILI934 ��ʾ�������ʼ������������� ***************************/
#define      ILI9341_DispWindow_X_Star		    0     //��ʼ���X����
#define      ILI9341_DispWindow_Y_Star		    0     //��ʼ���Y����


#define ILI9341_LESS_PIXEL 240			//Һ�����϶̷�������ؿ��
#define ILI9341_MORE_PIXEL 320			//Һ�����ϳ���������ؿ��

//����Һ��ɨ�跽����仯��XY���ؿ�ȣ�����ILI9341_GramScan�������÷���ʱ���Զ�����
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

//Һ����ɨ��ģʽѡֵΪ0-7
extern uint8_t LCD_SCAN_MODE;

/******************************* ���� ILI934 ��ʾ��������ɫ ********************************/
#define      WHITE		0xFFFF	   //��ɫ
#define      BLACK      0x0000	   //��ɫ 
#define      GREY       0xF7DE	   //��ɫ 
#define      BLUE       0x001F	   //��ɫ 
#define      BLUE2      0x051F	   //ǳ��ɫ
#define		 BLUE_SPE	0x07FF	   //������
#define      RED        0xF800	   //��ɫ 
#define      MAGENTA    0xF81F	   //����ɫ�����ɫ 
#define      GREEN      0x07E0	   //��ɫ 
#define      CYAN       0x7FFF	   //����ɫ����ɫ 
#define      YELLOW     0xFFE0	   //��ɫ 
#define      BRED       0xF81F	   //��ɫ
#define      GRED       0xFFE0	   //����
#define      GBLUE      0x07FF	   //����ɫ

/******************************* ���� ILI934 �������� ********************************/
#define      CMD_SetCoordinateX		 		    0x2A	     //����X����
#define      CMD_SetCoordinateY		 		    0x2B	     //����Y����
#define      CMD_SetPixel		 		        0x2C	     //�������

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


//C99�﷨����������������ʹ��

extern __inline void                 ILI9341_Write_Cmd           ( uint16_t usCmd );
extern __inline void                 ILI9341_Write_Data          ( uint16_t usData );
extern __inline uint16_t             ILI9341_Read_Data           ( void );
extern __inline void          ILI9341_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );


/**
  * @brief  ��ILI9341д������
  * @param  usCmd :Ҫд��������Ĵ�����ַ��
  * @retval ��
  */	
__inline void ILI9341_Write_Cmd ( uint16_t usCmd )
{
	ILI9341_CS_CLR;//��ʼƬѡ      
	ILI9341_DC_CLR;//д����
	ILI9341_RD_SET;//��ֹ��
	DATAOUT(usCmd);//�������
	ILI9341_WR_CLR;//д�뿪ʼ
	ILI9341_WR_SET;//д�����
	ILI9341_CS_SET;//����Ƭѡ        	
}


/**
  * @brief  ��ILI9341д������
  * @param  usData :Ҫд�������
  * @retval ��
  */	
__inline void ILI9341_Write_Data ( uint16_t usData )
{
	ILI9341_CS_CLR;//��ʼƬѡ      
	ILI9341_DC_SET;//д����
	ILI9341_RD_SET;//��ֹ��
	DATAOUT(usData);//�������
	ILI9341_WR_CLR;//д�뿪ʼ
	ILI9341_WR_SET;//д�����
	ILI9341_CS_SET;//����Ƭѡ   
	
}


/**
  * @brief  ��ILI9341��ȡ����
  * @param  ��
  * @retval ��ȡ��������
  */	
__inline uint16_t ILI9341_Read_Data ( void )
{
	uint16_t data;
 	ILI9341_DATA_PORT->CRL=0X88888888; //��������
	ILI9341_DATA_PORT->CRH=0X88888888; //��������
	ILI9341_DATA_PORT->ODR=0X0000;     //ȫ�����0

	ILI9341_DC_SET;
	ILI9341_WR_SET;

	ILI9341_CS_CLR;
	//��ȡ����
	ILI9341_RD_CLR;    
    
	data=DATAIN;  
	ILI9341_RD_SET;
	ILI9341_CS_SET; 

	ILI9341_DATA_PORT->CRL=0X33333333; // �������
	ILI9341_DATA_PORT->CRH=0X33333333; // �������
	ILI9341_DATA_PORT->ODR=0XFFFF;    //ȫ�������
	return data;  
}

/**
 * @brief  ��ILI9341��ʾ������ĳһ��ɫ������ص�
 * @param  ulAmout_Point ��Ҫ�����ɫ�����ص������Ŀ
 * @param  usColor ����ɫ
 * @retval ��
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


