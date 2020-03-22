#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "schedule_app.h"
#include "menu_app.h"
#include "drv_key.h"
#include "steering_app.h"

extern enum MainSchStateType MainSchStep;
extern enum MainSchStateType MainSchStep_choose;
extern uint8_t menu_select_changedFlag;

void KEY1_DEV_Handle(void *args)
{   
	menu_select_changedFlag = 1;
		
	switch (MainSchStep)
	{
		case Interface_State:
			MainSchStep = Menu_State;
			break;
		
		case Menu_State:
			MainSchStep_choose+=1;
			if(MainSchStep_choose==(Type_Num+1))
			{
				MainSchStep_choose = CheckTouch_State;
			}
			break;
			
		case CheckTouch_State:
			
			break;
		
		case Palette_State:
			
			break;
		
		case Steering_State:
			
			break;
		
		case Type_Num:
			
			break;		
	}					
}

void KEY2_DEV_Handle(void *args)
{
	menu_select_changedFlag = 1;
	
	switch (MainSchStep)
	{
		case Interface_State:
			MainSchStep = Menu_State;
			break;
		
		case Menu_State:
			/* Enter Button */
			MainSchStep = MainSchStep_choose;
			break;
			
		case CheckTouch_State:
			
			break;
		
		case Palette_State:
			/* Back Button */
			MainSchStep = Menu_State;			
			break;
		
		case Steering_State:
			/* Back Button */
		
			/* release the steering control*/
			Steering_PWM_Disable();
		
			MainSchStep = Menu_State;
			break;
		
		case Type_Num:
			
			break;		
	}
}

void KEY_Init(void)
{
	/* 按键引脚为输入模式 */
    rt_pin_mode(KEY1_DEV, PIN_MODE_INPUT);
    /* 绑定中断，上升沿模式*/
    rt_pin_attach_irq(KEY1_DEV, PIN_IRQ_MODE_FALLING, KEY1_DEV_Handle, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY1_DEV, PIN_IRQ_ENABLE);
	
	/* 按键引脚为输入模式 */
    rt_pin_mode(KEY2_DEV, PIN_MODE_INPUT);
    /* 绑定中断，上升沿模式 */
    rt_pin_attach_irq(KEY2_DEV, PIN_IRQ_MODE_FALLING, KEY2_DEV_Handle, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY2_DEV, PIN_IRQ_ENABLE);	
}

