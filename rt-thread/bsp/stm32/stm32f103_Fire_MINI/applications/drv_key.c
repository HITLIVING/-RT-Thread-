#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "schedule_app.h"
#include "drv_key.h"

extern enum MainSchStateType MainSchStep;

void KEY1_DEV_Handle(void *args)
{   
    MainSchStep = CheckTouch_State;
}

void KEY2_DEV_Handle(void *args)
{	
	MainSchStep = MainSchStep==Palette_State ? Interface_State:Palette_State;
}

void KEY_Init(void)
{
	/* ��������Ϊ����ģʽ */
    rt_pin_mode(KEY1_DEV, PIN_MODE_INPUT);
    /* ���жϣ�������ģʽ*/
    rt_pin_attach_irq(KEY1_DEV, PIN_IRQ_MODE_FALLING, KEY1_DEV_Handle, RT_NULL);
    /* ʹ���ж� */
    rt_pin_irq_enable(KEY1_DEV, PIN_IRQ_ENABLE);
	
	/* ��������Ϊ����ģʽ */
    rt_pin_mode(KEY2_DEV, PIN_MODE_INPUT);
    /* ���жϣ�������ģʽ */
    rt_pin_attach_irq(KEY2_DEV, PIN_IRQ_MODE_FALLING, KEY2_DEV_Handle, RT_NULL);
    /* ʹ���ж� */
    rt_pin_irq_enable(KEY2_DEV, PIN_IRQ_ENABLE);	
}

