#ifndef __KALMAN_MATH_H__
#define __KALMAN_MATH_H__

struct KalmanParam
{
	float Last_P;	//上次估算协方差
	float Now_P;	//当前估算协方差
	float Kg;		//卡尔曼增益
	float Q;		//过程噪声协方差
	float R;		//观测噪声协方差
	float out;		//卡尔曼滤波器输出
};

void KalmanParamInit(struct KalmanParam* KalmanData, float Last_P,float Q,float R);
void KalmanParamUpdate(struct KalmanParam* KalmanData, float Input);



#endif //__KALMAN_MATH_H__


