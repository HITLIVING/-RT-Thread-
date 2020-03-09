#ifndef __DRV_XPT2049_LCD_H__
#define __DRV_XPT2049_LCD_H__

#ifndef __BSP_XPT2046_LCD_H
#define	__BSP_XPT2046_LCD_H

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "stm32f1xx_hal.h"


/******************************* XPT2046 �����������ź�ָʾ���Ŷ���(��ʹ���ж�) ***************************/
#define	XPT2046_PENIRQ			GET_PIN(C, 12)

//�����ź���Ч��ƽ
#define XPT2046_PENIRQ_ActiveLevel 0 //0��ƽ
#define	XPT2046_PENIRQ_Read()	rt_pin_read(XPT2046_PENIRQ)



/******************************* XPT2046 ������ģ��SPI���Ŷ��� ***************************/
#define	XPT2046_SPI_CS			GET_PIN(C, 9)

#define	XPT2046_SPI_CLK			GET_PIN(C, 8)

#define	XPT2046_SPI_MOSI		GET_PIN(C, 10)

#define	XPT2046_SPI_MISO		GET_PIN(C, 11)

#define XPT2046_CS_ENABLE()     rt_pin_write(XPT2046_SPI_CS, PIN_HIGH)   
#define XPT2046_CS_DISABLE()    rt_pin_write(XPT2046_SPI_CS, PIN_LOW)  

#define XPT2046_CLK_HIGH()      rt_pin_write(XPT2046_SPI_CLK, PIN_HIGH)     
#define XPT2046_CLK_LOW()       rt_pin_write(XPT2046_SPI_CLK, PIN_LOW)   

#define XPT2046_MOSI_1()        rt_pin_write(XPT2046_SPI_MOSI, PIN_HIGH)   
#define XPT2046_MOSI_0()        rt_pin_write(XPT2046_SPI_MOSI, PIN_LOW)  

#define XPT2046_MISO()          (rt_pin_read(XPT2046_SPI_MISO)==0x00000001?1:0)

/******************************* XPT2046 �������������� ***************************/
//У׼������ʱ���������ADֵ������� 
#define             XPT2046_THRESHOLD_CalDiff                    2               

#define	            XPT2046_CHANNEL_X 	                          0x90 	          //ͨ��Y+��ѡ�������	
#define	            XPT2046_CHANNEL_Y 	                          0xd0	          //ͨ��X+��ѡ�������

//��������д��FLASH��ı�־
#define							FLASH_TOUCH_PARA_FLAG_VALUE					0xA5

//������־д��FLASH��ĵ�ַ
#define 							FLASH_TOUCH_PARA_FLAG_ADDR						(1*1024)

//��������д��FLASH��ĵ�ַ
#define 							FLASH_TOUCH_PARA_ADDR									(2*1024)


/*��Ϣ���*/
#define XPT2046_DEBUG_ON         0

#define XPT2046_INFO(fmt,arg...)           printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define XPT2046_ERROR(fmt,arg...)          printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)
#define XPT2046_DEBUG(fmt,arg...)          do{\
                                          if(XPT2046_DEBUG_ON)\
                                          printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/******************************* ���� XPT2046 ��ص��������� ***************************/
typedef	struct          //Һ������ṹ�� 
{		
	/*����ֵ��ʾ��������*/
   int16_t x;			//��¼���µĴ�������ֵ
   int16_t y; 
	
	/*���ڼ�¼��������ʱ(����)����һ�δ���λ��*/
	 int16_t pre_x;		
   int16_t pre_y;
	
} strType_XPT2046_Coordinate;   


typedef struct         //У׼���ӽṹ�� 
{
	 float An,  		 //ע:sizeof(long double) = 8
					Bn,     
					Cn,   
					Dn,    
					En,    
					Fn,     
					Divider;
	
} strType_XPT2046_Calibration;


typedef struct         //У׼ϵ���ṹ�壨����ʹ�ã�
{
	 float dX_X,  			 
					dX_Y,     
					dX,   
					dY_X,    
					dY_Y,    
					dY;

} strType_XPT2046_TouchPara;

/******����״̬�����******/
typedef enum
{
	XPT2046_STATE_RELEASE  = 0,	//�����ͷ�
	XPT2046_STATE_WAITING,			//��������
	XPT2046_STATE_PRESSED,			//��������
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//����������ֵ
#define DURIATION_TIME				2

/******************************* ���� XPT2046 ��ص��ⲿȫ�ֱ��� ***************************/
extern volatile uint8_t               ucXPT2046_TouchFlag;

extern strType_XPT2046_TouchPara      strXPT2046_TouchPara[];


/******************************** XPT2046 �������������� **********************************/
void XPT2046_Init( void );
uint8_t XPT2046_Touch_Calibrate( uint8_t LCD_Mode );
uint8_t XPT2046_Get_TouchedPoint( strType_XPT2046_Coordinate * displayPtr, strType_XPT2046_TouchPara * para );
void XPT2046_TouchDown(strType_XPT2046_Coordinate * touch);
void XPT2046_TouchUp(strType_XPT2046_Coordinate * touch);
void XPT2046_TouchEvenHandler(void );

#endif /* __BSP_TOUCH_H */



#endif

