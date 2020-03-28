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
	//Ԥ��״̬����
	KalmanData->out=KalmanData->out;

	//Ԥ��Э�����
	KalmanData->Now_P=KalmanData->Last_P+KalmanData->Q;

	//���������淽��
	KalmanData->Kg=KalmanData->Now_P/(KalmanData->Now_P+KalmanData->R);

	//��������ֵ����
	KalmanData->out=KalmanData->out+KalmanData->Kg*(Input-KalmanData->out);

	//����Э�����
	KalmanData->Last_P=(1-KalmanData->Kg)*KalmanData->Now_P;

}













