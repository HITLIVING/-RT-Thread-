/*
 * Copyright (c) 2020 WangXi
 *
 * Gyroscope Drive and Algorithm
 *
 * Change Logs:
 *  
 * Note:
 * ����������Ҫ�ع�
 * ƫ���������
 * ���������ϵ
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gyroscope_app.h"
#include "drv_mpu9250.h"
#include "drv_ili9341_lcd.h"
#include "kalman_math.h"


//**********************  Gyroscope Data Sheet  **************************//
rt_int16_t chip_temp;        					//�¶�
rt_int16_t orignal_gx,orignal_gy,orignal_gz;    //�����ٶȼ�ԭʼֵ
rt_int16_t orignal_ax,orignal_ay,orignal_az;    //������ٶȼ�ԭʼֵ


#define OFFSET_SAMPLE_GA 1000					//ƫ�����������������
float offset_sample_ga = 0.0;					//ƫ�������������
float offset_gx,offset_gy,offset_gz;    		//�����ٶȼ�ƫ����
float offset_ax,offset_ay,offset_az;    		//������ٶȼ�ƫ����

float real_gx,real_gy,real_gz;    				//�����ٶȼ���ʵֵ����λ����� ��/�룩
float real_ax,real_ay,real_az;    				//������ٶȼ���ʵֵ����λ����� g��

float Angle_z = 0;								//Z��ת�����ֵ

//**********************  Magnetometer Data Sheet  **************************//

rt_int16_t orignal_mx,orignal_my,orignal_mz;    //���������ԭʼֵ

float offset_mx = (-129.0+93.0)/2.0;			//�شż�X�����
float offset_my = (14.0+244.0)/2.0;				//�شż�Y�����
float offset_mz = 0.0;							//�شż�Z�����

#define OFFSET_SAMPLE_MZ (OFFSET_SAMPLE_GA/10)	//�شżƳ�ʼ�Ǽ������������
float offset_sample_mz = 0.0;					//ƫ�������������
double offset_Angle_mz = 0.0;					//Z���ʼ�Ƕ�

double Angle_mz = 0.0;							//�����Ƽ���Z��ת�ǣ���λ����� �ȣ�

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

struct KalmanParam KalParam_mz;
#define	KalParam_mz_LastP 	0.02
#define	KalParam_mz_Q 	  	0.001
#define	KalParam_mz_R 	  	0.543

rt_uint32_t ms_Hz = 0;
void gyr_schedule(void)
{
	
//**********************  Sample Period test  **************************//	
	tick_now=rt_tick_get();
	tick_delta = tick_now-tick_last;
//	printf("%d\n",tick_delta);
	tick_last = tick_now;

//********************** Period deal ****************************//
	switch(gyr_step)
	{
		case gyr_sample:
			{			
				gyr_sample_dataGet();
				offset_sample_ga++;
				
				ms_Hz++;
				if(ms_Hz == 10)
				{
					mag_sample_dataGet();
					offset_sample_mz++;

					ms_Hz = 0;
				}
				
				if(offset_sample_ga>=OFFSET_SAMPLE_GA && offset_sample_mz>=OFFSET_SAMPLE_MZ)
					gyr_step = gyr_samplecul;
				
				break;
			}							
		case gyr_samplecul:
			{
				gyr_offset_cul();
				mag_offset_cul();
				gyr_step = gyr_deal;
				break;
			}			
		case gyr_deal:
			{
				gyr_data_deal();
				
				ms_Hz++;
				if(ms_Hz == 10)
				{
					mag_data_deal();

					ms_Hz = 0;
				}
				
				break;
			}			
		default:
			
			break;	
	}
}

void gyr_sample_dataGet(void)
{
	MPU_Get_Gyroscope(&orignal_gx, &orignal_gy, &orignal_gz);	
	
	offset_gz += orignal_gz;

}

void gyr_offset_cul(void)
{
	offset_gz = offset_gz/offset_sample_ga;

}

void gyr_data_deal(void)
{         
	MPU_Get_Gyroscope(&orignal_gx, &orignal_gy, &orignal_gz);	
		
	real_gz = (orignal_gz-offset_gz)/131.068;
	
	KalmanParamUpdate(&KalParam_gz, real_gz);
	
	Angle_z+=KalParam_gz.out*tick_delta*0.001;
	
//	printf("%.5f\n",Angle_z);
}

void mag_sample_dataGet(void)
{
	MPU_Get_Magnetometer(&orignal_mx,&orignal_my,&orignal_mz);
	
	Angle_mz = atan((orignal_my - offset_my)/(orignal_mx - offset_mx));
		
	offset_Angle_mz+=Angle_mz;
}

void mag_offset_cul(void)
{
	offset_Angle_mz = offset_Angle_mz/offset_sample_mz;
	
}

void mag_data_deal(void)
{
	MPU_Get_Magnetometer(&orignal_mx,&orignal_my,&orignal_mz);
	
	Angle_mz = atan((orignal_my - offset_my)/(orignal_mx - offset_mx)) - offset_Angle_mz;
	Angle_mz = Angle_mz/3.1415926*180.0;
	
	KalmanParamUpdate(&KalParam_mz, Angle_mz);		
	
	printf("%f\n",KalParam_mz.out);

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
	int res = 0;
	
	ILI9341_DispStringLine_EN (LINE(1), "Gyroscope Temperature:" );
	ILI9341_DispStringLine_EN (LINE(3), "Gyroscope Angular Speed:" );
	ILI9341_DispStringLine_EN (LINE(5), "Gyroscope Angular Accel:" );
	ILI9341_DispStringLine_EN (LINE(7), "Gyroscope Z Angular:" );
	
	/* init the mpu9250 drive */
	res = MPU9250_Init();
	if(res!=0)
		printf("mpu9250 drive error\n");
		
	rt_thread_mdelay(50);
	
	/* Display the temp of mpu9250*/
	char str_temp[5]={0};
	chip_temp =  MPU_Get_Temperature();
	sprintf(str_temp, "%d", chip_temp/100);
	ILI9341_DispStringLine_EN (LINE(2), str_temp);
	
	/* Init the Kalman Filter on g_Z*/
	KalmanParamInit(&KalParam_gz,KalParam_gz_LastP,KalParam_gz_Q,KalParam_gz_R);
		
	/* Init the Kalman Filter on m_Z*/
	KalmanParamInit(&KalParam_mz,KalParam_mz_LastP,KalParam_mz_Q,KalParam_mz_R);	
	
	/* Get the init Angle Z */
	MPU_Get_Magnetometer(&orignal_mx,&orignal_my,&orignal_mz);
	offset_Angle_mz = atan((orignal_my - offset_my)/(orignal_mx - offset_mx));
}










