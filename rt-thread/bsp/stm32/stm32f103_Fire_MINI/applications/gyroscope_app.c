
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#include "gyroscope_app.h"
#include "drv_mpu6050.h"
#include "drv_ili9341_lcd.h"

rt_int16_t temp;        						//温度
rt_int16_t orignal_gx,orignal_gy,orignal_gz;    //三轴加速度
rt_int16_t orignal_ax,orignal_ay,orignal_az;    //三轴角速度

void gyr_original_dataGet(void)
{ 
	mpu6050_temperature_get(&temp);
         
	mpu6050_accelerometer_get(&orignal_ax, &orignal_ay, &orignal_az);

	mpu6050_gyroscope_get(&orignal_gx, &orignal_gy, &orignal_gz);   
	
}

void gyr_dateDisplay(void)
{
	char str_temp[5]={0};
	char str_AnSpeed[50]={0};
	char str_AnAccel[50]={0};
	
	sprintf(str_temp, "%d", temp/100); 
	sprintf(str_AnSpeed, "x = %d y = %d z = %d", orignal_ax,orignal_ay,orignal_az);
	sprintf(str_AnAccel, "x = %d y = %d z = %d", orignal_gx,orignal_gy,orignal_gz);
	
	ILI9341_DispStringLine_EN (LINE(2), str_temp);
	ILI9341_DispStringLine_EN (LINE(4), str_AnSpeed);
	ILI9341_DispStringLine_EN (LINE(6), str_AnAccel);		
}

void gyroscope_init(void)
{
	ILI9341_DispStringLine_EN (LINE(1), "Gyroscope Temperature:" );
	ILI9341_DispStringLine_EN (LINE(3), "Gyroscope Angular Speed:" );
	ILI9341_DispStringLine_EN (LINE(5), "Gyroscope Angular Accel:" );	
	
	/* init the mpu6050 drive */
	mpu6050_hw_init();
}










