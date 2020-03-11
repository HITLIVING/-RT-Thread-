#ifndef __SCHEDULE_APP_H__
#define __SCHEDULE_APP_H__


enum MainSchStateType
{
	Interface_State,
	
	Menu_State,
	
	CheckTouch_State,
	
	Palette_State,
	
	Type_Num
};


void thread_System_Schedule_init(void);


#endif









