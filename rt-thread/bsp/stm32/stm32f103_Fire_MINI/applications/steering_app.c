#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#include "steering_app.h"
#include "drv_ili9341_lcd.h"

rt_uint32_t PWM1_freq = 2000;
rt_uint32_t PWM1_duty = 20;

rt_err_t Steering_PWM_init(void)
{
	rt_uint32_t PWM1_period = 1000000000/PWM1_freq;
	rt_uint32_t PWM1_pulse = PWM1_period/100*PWM1_duty;	
	
	/* PWM device handle */
	struct rt_device_pwm *pwm_dev;      

    /* find device */
    pwm_dev = (struct rt_device_pwm *)rt_device_find("pwm1");
	
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find pwm1 device!\n");
		
		return RT_ERROR;
    }

    /* set PWM period and pulse*/
    rt_pwm_set(pwm_dev, 1, PWM1_period, PWM1_pulse);
	
    /* enable PWM device */
    rt_pwm_enable(pwm_dev, 1);
	
	ILI9341_DispStringLine_EN (LINE(4),  "PWM Signal Freqence:");
	ILI9341_DispStringLine_EN (LINE(6),  "PWM Signal Duty:");
	
	return RT_EOK;
}

void Steering_PWM_Condition(void)
{
	char str_freq[] = {0};
	char str_duty[] = {0};
	
	sprintf(str_freq, "%d", PWM1_freq); 
	sprintf(str_duty, "%d", PWM1_duty); 
	
	ILI9341_DispStringLine_EN (LINE(5),  str_freq);
	ILI9341_DispStringLine_EN (LINE(7),  str_duty);
	
}

void Steering_PWM_Disable(void)
{
	/* PWM device handle */
	struct rt_device_pwm *pwm_dev;      

    /* find device */
    pwm_dev = (struct rt_device_pwm *)rt_device_find("pwm1");
	
	/* disable PWM device */
    rt_pwm_disable(pwm_dev, 1);

}


























