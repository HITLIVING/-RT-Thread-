#ifndef __LCD_APP__
#define __LCD_APP__

enum MainSchStateType
{
	Interface_State,
	
	CheckTouch_State,
	
	Type_Num
};


void interface_system_Init(void);
void interface_system_reset(void);
void interface_time_show(void);






#endif

