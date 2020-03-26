#ifndef __GYROSCOPE_APP_H__
#define __GYROSCOPE_APP_H__

enum gyr_state
{
	gyr_sample,
	
	gyr_samplecul,
	
	gyr_deal,
	
	
	
	gyr_num
};


void gyroscope_init(void);
void gyr_schedule(void);
void gyr_sample_cul(void);
void gyr_data_deal(void);

void gyr_sample_dataGet(void);

















#endif // __GYROSCOPE_APP_H__

