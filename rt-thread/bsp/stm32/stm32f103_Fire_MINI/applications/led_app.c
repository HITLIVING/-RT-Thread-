#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define GPIONum_LED1 GET_PIN(C, 2)	//��ȡC2���ź�
#define GPIONum_LED2 GET_PIN(C, 3)	//��ȡC3���ź�

rt_bool_t LED1_Flag = PIN_HIGH;
rt_bool_t LED2_Flag = PIN_HIGH;

int LED_Init(void)
{
	//����GPIO����ģʽ
	rt_pin_mode( GPIONum_LED1, PIN_MODE_OUTPUT );
	rt_pin_mode( GPIONum_LED2, PIN_MODE_OUTPUT );
	rt_pin_write( GPIONum_LED1, LED1_Flag);
	rt_pin_write( GPIONum_LED2, LED2_Flag);
	return 0;
}
INIT_PREV_EXPORT(LED_Init);


rt_err_t LED1_Switch(void)
{	
	if(LED1_Flag == PIN_HIGH)
	{
		rt_pin_write( GPIONum_LED1, PIN_LOW);			//�������ŵ�ƽ
		LED1_Flag = PIN_LOW;
	}
	else
	{
		rt_pin_write( GPIONum_LED1, PIN_HIGH);			//�������ŵ�ƽ
		LED1_Flag = PIN_HIGH;
	}
	
    return RT_EOK;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(LED1_Switch, LED1 On/Off);


rt_err_t LED2_Switch(void)
{
	if(LED2_Flag == PIN_HIGH)
	{
		rt_pin_write( GPIONum_LED2, PIN_LOW);			//�������ŵ�ƽ
		LED2_Flag = PIN_LOW;
	}
	else
	{
		rt_pin_write( GPIONum_LED2, PIN_HIGH);			//�������ŵ�ƽ
		LED2_Flag = PIN_HIGH;
	}

    return RT_EOK;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(LED2_Switch, LED2 On/Off);


