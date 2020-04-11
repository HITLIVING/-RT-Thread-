#ifndef __GYROSCOPE_APP_H__
#define __GYROSCOPE_APP_H__

#include <rtthread.h>

enum gyr_state
{
	gyr_sample,
	
	gyr_samplecul,
	
	gyr_deal,
	
	
	
	gyr_num
};


void gyroscope_init(void);
void gyr_schedule(void);
void gyr_data_deal(int Debug);
void gyr_sample_dataGet(void);

void mag_data_deal(int Debug);
void mag_sample_dataGet(void);
double mag_AbsAngle_deal(double Angle,rt_int16_t ori_mx,rt_int16_t ori_my);

void GlobalPosition_AngleGet(float Alpha, int Debug);













#endif // __GYROSCOPE_APP_H__

