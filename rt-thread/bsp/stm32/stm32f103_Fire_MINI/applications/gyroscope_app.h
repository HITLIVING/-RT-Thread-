#ifndef __GYROSCOPE_APP_H__
#define __GYROSCOPE_APP_H__


//三轴速度计偏移值
#define init_gx_30 -43
#define init_gy_30 0
#define init_gz_30 31

//三轴加速度偏移值
#define init_ax_30 95
#define init_ay_30 -64
#define init_az_30 94

void gyroscope_init(void);
void gyr_original_dataGet(void);
void gyr_dateDisplay(void);

















#endif // __GYROSCOPE_APP_H__

