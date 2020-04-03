#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "gyroscope_dmp_app.h"
#include "drv_mpu9250.h"
#include "drv_ili9341_lcd.h"
#include "inv_mpu.h"

float pitch,roll,yaw; 	        //欧拉角
short aacx,aacy,aacz;	        //加速度传感器原始数据
short gyrox,gyroy,gyroz;        //陀螺仪原始数据
short magx,magy,magz;			//磁力计原始数据
short temp_DMP;		            //温度


void gyroscope_dmp_init(void)
{
	int res;
	
	ILI9341_DispStringLine_EN (LINE(1), "Gyroscope Temperature:" );
	ILI9341_DispStringLine_EN (LINE(3), "Gyroscope Angular Speed:" );
	ILI9341_DispStringLine_EN (LINE(5), "Gyroscope Accel:" );
	ILI9341_DispStringLine_EN (LINE(7), "Gyroscope magnetometer:" );
	ILI9341_DispStringLine_EN (LINE(9), "Gyroscope euler angle:" );
	/* init the mpu9250 drive */
	//MPU9250_Init();
	
	/* init the mpu9250_DMP drive */
	I2C_Dev_Init();	
	while(1)         
	{   
		res = mpu_dmp_init();
		if(res==0)
		{
			break;
		}
		else
		{
			rt_kprintf("mpu9250_DMP init error: errID_ %d  \n",res);
		}
	}
	
	/* Display the temp of mpu9250*/
	char str_temp[5]={0};
	temp_DMP=MPU_Get_Temperature();
	sprintf(str_temp, "%d", temp_DMP/100);
	ILI9341_DispStringLine_EN (LINE(2), str_temp);
	
}

void gyrDMP_sample_dataGet(void)
{
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	MPU_Get_Magnetometer(&magx,&magy,&magz);
	
	char str_AnSpeed[50]={0};
	char str_AnAccel[50]={0};
	char str_magnet[50]={0};
	
	sprintf(str_AnSpeed, "%d %d %d", gyrox,gyroy,gyroz);
	sprintf(str_AnAccel, "%d %d %d", aacx,aacy,aacz);
	sprintf(str_magnet, "%d %d %d", magx,magy,magz);	
	
	ILI9341_DispStringLine_EN (LINE(4), str_AnSpeed );
	ILI9341_DispStringLine_EN (LINE(6), str_AnAccel );
	ILI9341_DispStringLine_EN (LINE(8), str_magnet );
}

void gyrDMP_euangle_dataGet(void)
{
	mpu_mpl_get_data(&pitch,&roll,&yaw);
	
//	char str_euangle[50]={0};
//	
//	sprintf(str_euangle, "%f %f %f", pitch,roll,yaw);
//	
//	ILI9341_DispStringLine_EN (LINE(10), str_euangle);
	
	printf("%f\n",yaw);
}






















