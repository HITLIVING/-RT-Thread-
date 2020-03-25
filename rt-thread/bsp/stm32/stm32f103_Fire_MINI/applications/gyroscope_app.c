
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#include "gyroscope_app.h"
#include "drv_mpu6050.h"
#include "drv_ili9341_lcd.h"

rt_int16_t temp;        						//温度
rt_int16_t orignal_gx,orignal_gy,orignal_gz;    //三轴速度计原始值
rt_int16_t orignal_ax,orignal_ay,orignal_az;    //三轴加速度计原始值

rt_int16_t real_gx,real_gy,real_gz;    //三轴速度计真实值（单位换算后 度/秒 并放大100倍）
rt_int16_t real_ax,real_ay,real_az;    //三轴加速度计原始值（单位换算后 g     并放大100倍）



void gyr_original_dataGet(void)
{ 
	mpu6050_temperature_get(&temp);
         
	mpu6050_accelerometer_get(&orignal_ax, &orignal_ay, &orignal_az);

	mpu6050_gyroscope_get(&orignal_gx, &orignal_gy, &orignal_gz);  
	
	orignal_gx-=init_gx_30;
	orignal_gy-=init_gy_30;
	orignal_gz-=init_gz_30;
	orignal_ax-=init_ax_30;
	orignal_ay-=init_ay_30;
	orignal_az-=init_az_30;
	
	real_gx = orignal_gx*100/16.4;
	real_gy = orignal_gy*100/16.4;
	real_gz = orignal_gz*100/16.4;
	
	real_ax = orignal_ax*100/2048;
	real_ay = orignal_ay*100/2048;
	real_az = orignal_az*100/2048;
	
	rt_kprintf("%d,%d,%d,%d,%d,%d\n",real_gx,real_gy,real_gz,real_ax,real_ay,real_az);
	
}

void gyr_dateDisplay(void)
{
	char str_temp[5]={0};
	char str_AnSpeed[50]={0};
	char str_AnAccel[50]={0};
	
	sprintf(str_temp, "%d", temp/100); 
	sprintf(str_AnSpeed, "x = %d y = %d z = %d", orignal_ax,orignal_ay,orignal_az);
	sprintf(str_AnAccel, "x = %d y = %d z = %d", orignal_gx,orignal_gy,orignal_gz);
	
	LCD_ClearLine(LINE(2));
	ILI9341_DispStringLine_EN (LINE(2), str_temp);
	LCD_ClearLine(LINE(4));
	ILI9341_DispStringLine_EN (LINE(4), str_AnSpeed);
	LCD_ClearLine(LINE(6));
	ILI9341_DispStringLine_EN (LINE(6), str_AnAccel);
}

void gyroscope_init(void)
{
	ILI9341_DispStringLine_EN (LINE(1), "Gyroscope Temperature:" );
	ILI9341_DispStringLine_EN (LINE(3), "Gyroscope Angular Speed:" );
	ILI9341_DispStringLine_EN (LINE(5), "Gyroscope Angular Accel:" );	
	
	/* init the mpu6050 drive */
	mpu6050_hw_init();
	
	rt_thread_mdelay(50);
	
}










