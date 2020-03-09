#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "schedule_app.h"
#include "interface_app.h"
#include "message_app.h"
#include "drv_ili9341_lcd.h"
#include "drv_xpt2049_lcd.h"
#include "drv_key.h"


typedef enum MainSchStateType 	MainSch_State;
typedef MainSch_State 			(*MainSch_Procedure)(void);
enum MainSchStateType MainSchStep;


/*****************Main Schedule Procedure**********************/
MainSch_State step_Interface(void)
{
	interface_time_show();	
	
	if(MainSchStep!=Interface_State)
	{
		return MainSchStep;
	}
	return Interface_State;
}

MainSch_State step_CheckTouch(void)
{
	/* disable the interrupt from key1 */
    rt_pin_irq_enable(KEY1_DEV, PIN_IRQ_DISABLE);
	
	message_CheckPrepare();
	rt_thread_delay(1000);
	XPT2046_Touch_Calibrate(6);
	interface_system_reset();
	
	/* enable the interrupt from key1 */
    rt_pin_irq_enable(KEY1_DEV, PIN_IRQ_ENABLE);
	
	return Interface_State;
}

MainSch_State step_Num(void)
{
	
	
	return Interface_State;
}


MainSch_Procedure State_ProceSteps[] = 
{ 
	step_Interface,
	
	step_CheckTouch,
	
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

#define MAIN_THREAD_PRIORITY         25
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

