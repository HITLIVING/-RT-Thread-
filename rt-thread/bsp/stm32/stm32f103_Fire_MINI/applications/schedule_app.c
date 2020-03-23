#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "schedule_app.h"
#include "interface_app.h"
#include "message_app.h"
#include "menu_app.h"
#include "steering_app.h"

#include "drv_ili9341_lcd.h"
#include "drv_xpt2049_lcd.h"
#include "drv_key.h"

typedef enum MainSchStateType 	MainSch_State;
typedef MainSch_State 			(*MainSch_Procedure)(void);
enum MainSchStateType MainSchStep = Interface_State;
enum MainSchStateType Last_MainSchStep = Type_Num;

/*****************Main Schedule Procedure**********************/
MainSch_State step_Interface(void)
{
	/* step init */
	if(Last_MainSchStep!=MainSchStep)
	{
		ILI9341_Clear (0, 0, 240, 320);
		message_SysReaday();
		message_toMenu();		
	}
	Last_MainSchStep = MainSchStep;
	
	/*Interface Application*/
	interface_time_show();	
	
	return MainSchStep;
}

MainSch_State step_Menu(void)
{
	/* step init */
	if(Last_MainSchStep!=MainSchStep)
	{
		ILI9341_Clear (0, 0, 240, 320);
		LCD_SetColors(BLUE_SPE, BLACK); 
		message_Menu();
		menu_init();		
	}
	Last_MainSchStep = MainSchStep;
	
	menu_select();	
	menu_timeshow();
	
	
	return MainSchStep;
}

MainSch_State step_CheckTouch(void)
{
	/* step init */
	if(Last_MainSchStep!=MainSchStep)
	{
		ILI9341_Clear (0, 0, 240, 320);
		message_CheckPrepare();
	}
	Last_MainSchStep = MainSchStep;
	
	/* disable the interrupt from key1 */
    rt_pin_irq_enable(KEY1_DEV, PIN_IRQ_DISABLE);
	
	/* Touch Check Application*/
	XPT2046_Touch_Calibrate(6);
	
	/* enable the interrupt from key1 */
    rt_pin_irq_enable(KEY1_DEV, PIN_IRQ_ENABLE);
	
	/* Check finished go back to Interface */
	return Menu_State;
}

MainSch_State step_Palette(void)
{
	/* step init */
	if(Last_MainSchStep!=MainSchStep)
	{
		ILI9341_Clear (0, 0, 240, 320);
		message_BackToMenu();
	}
	Last_MainSchStep = MainSchStep;

	/* Palette Application */
	XPT2046_TouchEvenHandler( );
	
	return MainSchStep;
}

MainSch_State step_Steering(void)
{
	/* step init */
	if(Last_MainSchStep!=MainSchStep)
	{
		ILI9341_Clear (0, 0, 240, 320);		
		message_BackToMenu();
		/*Enable the Steering*/
		Steering_PWM_init();
	}
	Last_MainSchStep = MainSchStep;

	Steering_PWM_Condition();

	return MainSchStep;
}

MainSch_State step_Num(void)
{
	
	
	return Interface_State;
}


/*****************Main Schedule ProceSteps Init**********************/
MainSch_Procedure State_ProceSteps[] = 
{ 
	step_Interface,
	
	step_Menu,
	
	step_CheckTouch,
	
	step_Palette,
	
	step_Steering,
	
	step_Num

};



/**
  * @brief  System main Schedule Thread Entry
  * @param	None
  * @return None
  * @note	Provide a timescale
  */
static void thread_main_entry(void *parameter)
{		
	while(1)
	{
		rt_thread_mdelay(1);
		MainSchStep = (*State_ProceSteps[MainSchStep])();					
	}		
}

#define MAIN_THREAD_PRIORITY         21
#define MAIN_THREAD_STACK_SIZE       1024
#define MAIN_THREAD_TIMESLICE        100
/**
  * @brief  System main Schedule Thread Init
  * @param	None
  * @return None
  * @note	Provide a timescale
  */
ALIGN(RT_ALIGN_SIZE)
void thread_System_Schedule_init(void)
{
	rt_thread_t thread_main_obj = RT_NULL;
    /* Build a thread */
    thread_main_obj = rt_thread_create("thread_System_Schedule",
                            thread_main_entry, 
							RT_NULL,
                            MAIN_THREAD_STACK_SIZE,
                            MAIN_THREAD_PRIORITY, 
							MAIN_THREAD_TIMESLICE);

    /* If get the thread object, start it */
    if (thread_main_obj != RT_NULL)
	{
		rt_thread_startup(thread_main_obj);
	}      
	else
	{
		error_SchFailed();
	}
}

