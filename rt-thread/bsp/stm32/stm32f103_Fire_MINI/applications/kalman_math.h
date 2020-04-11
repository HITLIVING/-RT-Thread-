#ifndef __KALMAN_MATH_H__
#define __KALMAN_MATH_H__

struct KalmanParam
{
	float Last_P;	//�ϴι���Э����
	float Now_P;	//��ǰ����Э����
	float Kg;		//����������
	float Q;		//��������Э����
	float R;		//�۲�����Э����
	float out;		//�������˲������
};

void KalmanParamInit(struct KalmanParam* KalmanData, float Last_P,float Q,float R);
void KalmanParamUpdate(struct KalmanParam* KalmanData, float Input);



#endif //__KALMAN_MATH_H__


