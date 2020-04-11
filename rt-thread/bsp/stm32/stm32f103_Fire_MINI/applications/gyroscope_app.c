/*
 * Copyright (c) 2020 WangXi
 *
 * Gyroscope Drive and Algorithm
 *
 * Change Logs:
 *  
 * 
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gyroscope_app.h"
#include "drv_mpu9250.h"
#include "drv_ili9341_lcd.h"

#include "algorythm_math.h"
#include "kalman_math.h"



//**********************  User Data Sheet  **************************//
#define LCD_USING
#define Angle_180Degree

float GlobalPosition_Angle = 0.0;				//����ȫ�ֶ�λת��


//**********************  Gyroscope Data Sheet  **************************//
rt_int16_t chip_temp;        					//�¶�
rt_int16_t orignal_gx,orignal_gy,orignal_gz;    //�����ٶȼ�ԭʼֵ
rt_int16_t orignal_ax,orignal_ay,orignal_az;    //������ٶȼ�ԭʼֵ

#define OFFSET_SAMPLE_GA 1000					//ƫ�����������������
float offset_sample_ga = 0.0;					//ƫ�������������
float offset_gx,offset_gy,offset_gz;    		//�����ٶȼ�ƫ����

float Angle_gz = 0;								//�ٶȼ�Z��ת�Ǽ���ֵ

//**********************  Magnetometer Data Sheet  **************************//

rt_int16_t orignal_mx,orignal_my,orignal_mz;    //���������ԭʼֵ

//Data from Matlab
#define offset_mx  -12.447578					//�شż�X�����
#define offset_my  35.371766					//�شż�Y�����
#define offset_mr  115.544546/124.374368		//�شż�Բ������x/y

#define OFFSET_SAMPLE_MZ (OFFSET_SAMPLE_GA/10)	//�شżƳ�ʼ�Ǽ������������
float offset_sample_mz = 0.0;					//ƫ�������������
double offset_Angle_mz = 0.0;					//Z���ʼ�Ƕ�

double Angle_mz = 0.0;							//�����Ƽ���Z��ת�ǣ���λ����� �ȣ�

//**********************  Sample Period (dt)  **************************//
rt_tick_t tick_delta = 0;

//**********************  Gyroscope Schedule Flag  **************************//
rt_int16_t gyr_step = gyr_sample;

//**********************  Kalman Filter Struct  **************************//
struct KalmanParam KalParam_gz;
#define	KalParam_gz_LastP 	0.02
#define	KalParam_gz_Q 	  	0.001
#define	KalParam_gz_R 	  	0.543

struct KalmanParam KalParam_mx;
#define	KalParam_mx_LastP 	0.02
#define	KalParam_mx_Q 	  	0.001
#define	KalParam_mx_R 	  	0.543

struct KalmanParam KalParam_my;
#define	KalParam_my_LastP 	0.02
#define	KalParam_my_Q 	  	0.001
#define	KalParam_my_R 	  	0.543

/**
  * @brief  ���������������
  * @note   
  * @param  None			
  * @retval 
  */
rt_uint32_t ms_Hz = 0;
void gyr_schedule(void)
{
	static rt_tick_t tick_now;
	static rt_tick_t tick_last;
	
	//Sample Period ��dt��	
	tick_now=rt_tick_get();
	tick_delta = tick_now-tick_last;
	tick_last = tick_now;

	//Period deal
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
				offset_gz = offset_gz/offset_sample_ga;
				offset_Angle_mz = offset_Angle_mz/offset_sample_mz;
				
				gyr_step = gyr_deal;
				break;
			}			
		case gyr_deal:
			{
				gyr_data_deal(0);
				
				ms_Hz++;
				if(ms_Hz == 10)
				{
					mag_data_deal(1);

					ms_Hz = 0;
				}
				
				GlobalPosition_AngleGet(0.9, 0);
				
				break;
			}			
		default:
			
			break;	
	}
}

/**
  * @brief  ǰ���ٶȼƲ����ۼ�ƽ����Ϊƫ��
  * @note   
  * @param  None			
  * @retval �ۼ�ֵ������ offset_gz
  */
void gyr_sample_dataGet(void)
{
	MPU_Get_Gyroscope(&orignal_gx, &orignal_gy, &orignal_gz);	
	
	offset_gz += orignal_gz;

}

/**
  * @brief  ��ȡ�ٶȼƴ����ĽǶ�ֵ
  * @note   ������Kalman�˲�����
  * @param  Debug = 1����ӡ��������ֵ��
			Debug = 0������ӡ
  * @retval �Ƕ�ֵ������ȫ�ֱ��� Angle_z
  */
void gyr_data_deal(int Debug)
{   
	MPU_Get_Gyroscope(&orignal_gx, &orignal_gy, &orignal_gz);	
		
	float  real_gz = (orignal_gz-offset_gz)/131.068;
	
	KalmanParamUpdate(&KalParam_gz, real_gz);
	
	Angle_gz-=KalParam_gz.out*tick_delta*0.001;
	Angle_gz = fmod(Angle_gz,360.0);
	
	//negetive angle changed to positive angle
	if(Angle_gz < 0)
	{
		Angle_gz += 360.0;
	}
	
#ifdef Angle_180Degree	
	if(Angle_gz > 180.0)
	{
		Angle_gz -= 360;
	}
#endif
	
	if(Debug)
		printf("%.5f\n",Angle_gz);
}

/**
  * @brief  �õ��شżƷ�λ�Ƕ�
  * @note   ������
  * @param  Angle��ԭʼ�Ƕ�
			ori_mx��X���ǿֵ
			ori_my��Y���ǿֵ
  * @retval AbsAngle�������ĽǶȣ�0~2PI��˳ʱ�룩
  */
double mag_AbsAngle_deal(double Angle,rt_int16_t ori_mx,rt_int16_t ori_my)
{
	double AbsAngle = 0.0;
	
	if(ori_mx>0 && ori_my>0)
	{
		AbsAngle = 2*PI - Angle;
	}
	else if(ori_mx<0 && ori_my>0)
	{
		AbsAngle = PI - Angle;
	}
	else if(ori_mx<0 && ori_my<0)
	{
		AbsAngle = PI - Angle;
	}	
	else if(ori_mx>0 && ori_my<0)
	{
		AbsAngle = - Angle;
	}
	else if(ori_mx==0 && ori_my>0)
	{
		AbsAngle = PI/2.0*3.0;
	}
	else if(ori_mx==0 && ori_my<0)
	{
		AbsAngle = PI/2.0;
	}
	else if(ori_mx<0 && ori_my==0)
	{
		AbsAngle = PI;
	}	
	
	return AbsAngle;
}

/**
  * @brief  ��ʼ��λ�ǲ����ۼ�
  * @note   ������
  * @param  None			
  * @retval �ۼ�ֵ������ offset_Angle_mz
  */
void mag_sample_dataGet(void)
{
	MPU_Get_Magnetometer(&orignal_mx,&orignal_my,&orignal_mz);
	
	Angle_mz = atan(offset_mr*(orignal_my - offset_my)/(orignal_mx - offset_mx));
	
	Angle_mz = mag_AbsAngle_deal(Angle_mz,(orignal_mx - offset_mx),(orignal_my - offset_my));
		
	offset_Angle_mz+=Angle_mz;
}


/**
  * @brief  ��ȡ�شżƴ����ĽǶ�ֵ
  * @note   ������Kalman�˲�����
  * @param  Debug = 1����ӡ��������ֵ
			Debug = 2����ӡ�������ԭʼ��������Matlab��Բ���
			Debug = 3, ��ӡ����������
			Debug = 0������ӡ		
  * @retval �Ƕ�ֵ������ȫ�ֱ��� Angle_mz
  */
void mag_data_deal(int Debug)
{
	MPU_Get_Magnetometer(&orignal_mx,&orignal_my,&orignal_mz);
	
	orignal_mx -= offset_mx;
	orignal_my -= offset_my;
	orignal_my *= offset_mr;
	
	KalmanParamUpdate(&KalParam_mx, orignal_mx);		
	KalmanParamUpdate(&KalParam_my, orignal_my);		
	
	Angle_mz = atan(KalParam_my.out / KalParam_mx.out);
	Angle_mz = mag_AbsAngle_deal(Angle_mz,KalParam_mx.out,KalParam_my.out);
	
	if(Angle_mz < offset_Angle_mz)
	{
		Angle_mz =2*PI - (offset_Angle_mz - Angle_mz);
	}
	else
	{
		Angle_mz = Angle_mz - offset_Angle_mz;
	}
	
	Angle_mz = Angle_mz/3.1415926*180.0;

#ifdef Angle_180Degree	
	if(Angle_mz > 180.0)
	{
		Angle_mz -= 360;
	}
#endif	
	
	if(Debug==1)
		printf("%f\n",Angle_mz);
	else if(Debug==2)
		printf("%d,%d,%d\n",orignal_mx,orignal_my,orignal_mz);
	else if(Debug==3)
		printf("%f,%f\n",KalParam_mx.out,KalParam_my.out);
}

/**
  * @brief  �ٶȼƺ͵شżƵĻ����˲�
  * @note  
  * @param  Alpha    , �˲���ϵ��
  * @param  Debug = 1����ӡ���սǶ���ֵ��
			Debug = 2����ӡ�ٶȼƽǶȣ��شżƽǶȣ����սǶ�������ֵ��
			Debug = 0������ӡ
  * @retval �Ƕ�ֵ������ȫ�ֱ��� GlobalPosition_Angle
  */
void GlobalPosition_AngleGet(float Alpha, int Debug)
{
	GlobalPosition_Angle = Alpha * Angle_gz + (1-Alpha) * Angle_mz;
	
#ifdef Angle_180Degree	
	if(GlobalPosition_Angle > 180.0)
	{
		GlobalPosition_Angle -= 360;
	}
#endif
	
	if(Debug==1)
		printf("%f\n",GlobalPosition_Angle);
	else if(Debug==2)
		printf("%f,%f,%f\n",Angle_gz,Angle_mz,GlobalPosition_Angle);
}


#ifdef LCD_USING
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
#endif

void gyroscope_init(void)
{
	/* init the mpu9250 drive */
	int res = MPU9250_Init();
	if(res!=0)
		printf("mpu9250 drive error\n");
		
	rt_thread_mdelay(50);
	
#ifdef LCD_USING	
	ILI9341_DispStringLine_EN (LINE(1), "Gyroscope Temperature:" );
	ILI9341_DispStringLine_EN (LINE(3), "Gyroscope Angular Speed:" );
	ILI9341_DispStringLine_EN (LINE(5), "Gyroscope Angular Accel:" );
	ILI9341_DispStringLine_EN (LINE(7), "Gyroscope Z Angular:" );

	/* Display the temp of mpu9250*/
	char str_temp[5]={0};
	chip_temp =  MPU_Get_Temperature();
	sprintf(str_temp, "%d", chip_temp/100);
	ILI9341_DispStringLine_EN (LINE(2), str_temp);
#endif
		
	/* Init the Kalman Filter on g_Z*/
	KalmanParamInit(&KalParam_gz,KalParam_gz_LastP,KalParam_gz_Q,KalParam_gz_R);
		
	/* Init the Kalman Filter on m_X*/
	KalmanParamInit(&KalParam_mx,KalParam_mx_LastP,KalParam_mx_Q,KalParam_mx_R);

	/* Init the Kalman Filter on m_Y*/
	KalmanParamInit(&KalParam_my,KalParam_my_LastP,KalParam_my_Q,KalParam_my_R);	

}










