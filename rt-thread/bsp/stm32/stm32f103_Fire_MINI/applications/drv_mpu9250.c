#include <rtthread.h>
#include <rtdevice.h>
#include "drv_mpu9250.h"

#define MPU9250_I2CBUS_NAME  "i2c1"     /* I2C�豸����,�����drv_i2c.cע���I2C�豸����һ�� */

static struct rt_i2c_bus_device *mpu9250_i2c_bus;    /* I2C�豸��� */

rt_uint8_t I2C_Dev_Init(void)
{
    mpu9250_i2c_bus = rt_i2c_bus_device_find(MPU9250_I2CBUS_NAME);  /*����I2C�豸*/

    if (mpu9250_i2c_bus == RT_NULL) 
    {
        rt_kprintf("can't find mpu6050 %s device\r\n",MPU9250_I2CBUS_NAME);
        return RT_ERROR;
    }

    rt_kprintf("mpu6050 set i2c bus to %s\r\n", MPU9250_I2CBUS_NAME);
	return RT_EOK;
}


rt_uint8_t MPU9250_Init(void)
{
    rt_uint8_t ID=0; 
	
	//Init the I2C device MPU9250 used;
	I2C_Dev_Init();
	
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU9250
	
    rt_thread_mdelay(100);  //��ʱ100ms,ʹ��Rt-thread�Դ���ʱ
	
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);//����MPU9250
    MPU_Set_Gyro_Fsr(3);					        	//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					       	 	//���ٶȴ�����,��2g
    MPU_Set_Rate(50);						       	 	//���ò�����50Hz
    MPU_Write_Byte(MPU9250_ADDR,MPU_INT_EN_REG,0X00);   //�ر������ж�
	MPU_Write_Byte(MPU9250_ADDR,MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU9250_ADDR,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	MPU_Write_Byte(MPU9250_ADDR,MPU_INTBP_CFG_REG,0X82);//INT���ŵ͵�ƽ��Ч������bypassģʽ������ֱ�Ӷ�ȡ������
    ID=MPU_Read_Byte(MPU9250_ADDR,MPU_DEVICE_ID_REG);  //��ȡMPU6500��ID
    if(ID==MPU6500_ID) //����ID��ȷ
    {
        MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X01);  	//����CLKSEL,PLL X��Ϊ�ο�
        MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT2_REG,0X00);  	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						       	//���ò�����Ϊ50Hz   
    }else return 1;
 
    ID=MPU_Read_Byte(AK8963_ADDR,MAG_WIA);    			//��ȡAK8963 ID   
    if(ID==AK8963_ID)
    {
        MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11);		//����AK8963Ϊ���β���ģʽ
    }else return 1;

    return 0;
}

//����MPU9250�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
rt_uint8_t MPU_Set_Gyro_Fsr(rt_uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU9250���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
rt_uint8_t MPU_Set_Accel_Fsr(rt_uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}

//����MPU9250�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
rt_uint8_t MPU_Set_LPF(rt_uint16_t lpf)
{
	rt_uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU9250_ADDR,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

//����MPU9250�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
rt_uint8_t MPU_Set_Rate(rt_uint16_t rate)
{
	rt_uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    rt_uint8_t buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU9250_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((rt_uint16_t)buf[0]<<8)|buf[1];  
    temp=21+((double)raw)/333.87;  
    return temp*100;
}

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
rt_uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    rt_uint8_t buf[6],res; 
	res=MPU_Read_Len(MPU9250_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((rt_uint16_t)buf[0]<<8)|buf[1];  
		*gy=((rt_uint16_t)buf[2]<<8)|buf[3];  
		*gz=((rt_uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
rt_uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    rt_uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((rt_uint16_t)buf[0]<<8)|buf[1];  
		*ay=((rt_uint16_t)buf[2]<<8)|buf[3];  
		*az=((rt_uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}

//�õ�������ֵ(ԭʼֵ)
//mx,my,mz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
rt_uint8_t MPU_Get_Magnetometer(short *mx,short *my,short *mz)
{
    rt_uint8_t buf[6],res;  
	res=MPU_Read_Len(AK8963_ADDR,MAG_XOUT_L,6,buf);
	if(res==0)
	{
		*mx=((rt_uint16_t)buf[1]<<8)|buf[0];  
		*my=((rt_uint16_t)buf[3]<<8)|buf[2];  
		*mz=((rt_uint16_t)buf[5]<<8)|buf[4];
	} 	
    MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963ÿ�ζ����Ժ���Ҫ��������Ϊ���β���ģʽ
    return res;
}

//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
rt_uint8_t MPU_Write_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf_in)
{
    rt_uint8_t buf[len+1];

    buf[0] = reg;
	
	for(rt_uint8_t i=0;i<len;i++)
    {
        buf[i+1] = buf_in[i];
    }

	rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, buf ,len+1);
       
	return 0;
} 

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
rt_uint8_t MPU_Read_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf)
{ 
	rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, &reg, 1);
	rt_i2c_master_recv(mpu9250_i2c_bus, addr, 0, buf, len);
	
	return 0;
}

//IICдһ���ֽ� 
//devaddr:����IIC��ַ
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
rt_uint8_t MPU_Write_Byte(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t data)
{
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = data;
    rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, buf ,2);
	
	return 0;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
rt_uint8_t MPU_Read_Byte(rt_uint8_t addr,rt_uint8_t reg)
{
	rt_uint8_t buf;
	rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, &reg, 1);
	rt_i2c_master_recv(mpu9250_i2c_bus, addr, 0, &buf, 1);
	return buf;
}




















