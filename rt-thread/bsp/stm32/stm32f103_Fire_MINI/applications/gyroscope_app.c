
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <stdlib.h>

#include "gyroscope_app.h"
#include "drv_mpu6050.h"
#include "drv_ili9341_lcd.h"
#include "kalman_math.h"


//**********************  Gyroscope Data Sheet  **************************//

rt_int16_t temp;        						//温度
rt_int16_t orignal_gx,orignal_gy,orignal_gz;    //三轴速度计原始值
rt_int16_t orignal_ax,orignal_ay,orignal_az;    //三轴加速度计原始值

#define OFFSET_SAMPLE 1000
float offset_sample = 0;					//偏移量计算采样数
float offset_gx,offset_gy,offset_gz;    	//三轴速度计偏移量
float offset_ax,offset_ay,offset_az;    	//三轴加速度计偏移量

float real_gx,real_gy,real_gz;    	//三轴速度计真实值（单位换算后 度/秒 并放大100倍）
float real_ax,real_ay,real_az;    	//三轴加速度计原始值（单位换算后 g     并放大100倍）

float Angle_z = 0;					//Z轴转角输出值

//**********************  Sample Period record  **************************//
rt_tick_t tick_now = 0;
rt_tick_t tick_last = 0;
rt_tick_t tick_delta = 0;

//**********************  Gyroscope Schedule Flag  **************************//
rt_int16_t gyr_step = gyr_sample;

//**********************  Kalman Filter Struct  **************************//
struct KalmanParam KalParam_gz;
#define	KalParam_gz_LastP 	0.02
#define	KalParam_gz_Q 	  	0.001
#define	KalParam_gz_R 	  	0.543


void gyr_schedule(void)
{
	
//**********************  Sample Period test  **************************//	
	tick_now=rt_tick_get();
	tick_delta = tick_now-tick_last;
//	printf("%d\n",tick_delta);
	tick_last = tick_now;
	
	switch(gyr_step)
	{
		case gyr_sample:
			{			
				gyr_sample_dataGet();
				offset_sample++;
				if(offset_sample==OFFSET_SAMPLE)
					gyr_step = gyr_samplecul;
				
				break;
			}							
		case gyr_samplecul:
			{
				gyr_sample_cul();
				gyr_step = gyr_deal;
				break;
			}			
		case gyr_deal:
			{
				gyr_data_deal();
				
				break;
			}			
		default:
			
			break;
		
	}
}

void gyr_sample_dataGet(void)
{
//	mpu6050_accelerometer_get(&orignal_ax, &orignal_ay, &orignal_az);

	mpu6050_gyroscope_get(&orignal_gx, &orignal_gy, &orignal_gz);
	
//	offset_ax += orignal_ax;
//	offset_ay += orignal_ay;
//	offset_az += orignal_az;
//	offset_gx += orignal_gx;
//	offset_gy += orignal_gy;
	offset_gz += orignal_gz;
	
//	rt_kprintf("%d\n",orignal_gz);

}

void gyr_sample_cul(void)
{
//	offset_ax = offset_ax/OFFSET_SAMPLE;
//	offset_ay = offset_ay/OFFSET_SAMPLE;
//	offset_az = offset_az/OFFSET_SAMPLE;
//	offset_gx = offset_gx/OFFSET_SAMPLE;
//	offset_gy = offset_gy/OFFSET_SAMPLE;
	offset_gz = offset_gz/OFFSET_SAMPLE;
	
//	printf("\n");
//	printf("%f\n",offset_gz);
}

void gyr_data_deal(void)
{         
//	mpu6050_accelerometer_get(&orignal_ax, &orignal_ay, &orignal_az);

	mpu6050_gyroscope_get(&orignal_gx, &orignal_gy, &orignal_gz);  
		
//	real_ax = orignal_ax/2048;
//	real_ay = orignal_ay/2048;
//	real_az = orignal_az/2048;
//	real_gx = orignal_gx/16.4;
//	real_gy = orignal_gy/16.4;
	real_gz = (orignal_gz-offset_gz)/65.534;
	
//	printf("%f\n",real_gz);
	
	KalmanParamUpdate(&KalParam_gz, real_gz);
	
//	printf("%f,%f\n",real_gz,KalParam_gz.out);
	
	Angle_z+=KalParam_gz.out*tick_delta*0.001;
	
	printf("%f\n",Angle_z);
}

void gyr_dateDisplay(void)
{
	char str_AnSpeed[50]={0};
	char str_AnAccel[50]={0};
	 
	sprintf(str_AnSpeed, "%d %d %d", orignal_ax,orignal_ay,orignal_az);
	sprintf(str_AnAccel, "%d %d %d", orignal_gx,orignal_gy,orignal_gz);

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
	ILI9341_DispStringLine_EN (LINE(7), "Gyroscope Z Angular:" );
	
	/* init the mpu6050 drive */
	mpu6050_hw_init();
	
	rt_thread_mdelay(50);
	
	/* Display the temp of mpu6050*/
	char str_temp[5]={0};
	mpu6050_temperature_get(&temp);
	sprintf(str_temp, "%d", temp/100);
	ILI9341_DispStringLine_EN (LINE(2), str_temp);
	
	/* Init the Kalman Filter on g_Z*/
	KalmanParamInit(&KalParam_gz,KalParam_gz_LastP,KalParam_gz_Q,KalParam_gz_R);
	
}










