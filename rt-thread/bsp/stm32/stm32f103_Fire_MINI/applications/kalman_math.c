#include "kalman_math.h"

void KalmanParamInit(struct KalmanParam* KalmanData, float Last_P,float Q,float R)
{
	KalmanData->Last_P = Last_P;
	KalmanData->Now_P = 0;
	KalmanData->Kg = 0;
	KalmanData->Q = Q;
	KalmanData->R = R;
	KalmanData->out = 0;

}

void KalmanParamUpdate(struct KalmanParam* KalmanData, float Input)
{
	//预测状态方程
	KalmanData->out=KalmanData->out;

	//预测协方差方程
	KalmanData->Now_P=KalmanData->Last_P+KalmanData->Q;

	//卡尔曼增益方程
	KalmanData->Kg=KalmanData->Now_P/(KalmanData->Now_P+KalmanData->R);

	//更新最优值方程
	KalmanData->out=KalmanData->out+KalmanData->Kg*(Input-KalmanData->out);

	//更新协方差方程
	KalmanData->Last_P=(1-KalmanData->Kg)*KalmanData->Now_P;

}













