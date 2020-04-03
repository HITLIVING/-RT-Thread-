#include <rtthread.h>
#include <rtdevice.h>
#include "drv_mpu9250.h"

#define MPU9250_I2CBUS_NAME  "i2c1"     /* I2C设备名称,必须和drv_i2c.c注册的I2C设备名称一致 */

static struct rt_i2c_bus_device *mpu9250_i2c_bus;    /* I2C设备句柄 */

rt_uint8_t I2C_Dev_Init(void)
{
    mpu9250_i2c_bus = rt_i2c_bus_device_find(MPU9250_I2CBUS_NAME);  /*查找I2C设备*/

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
	
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU9250
	
    rt_thread_mdelay(100);  //延时100ms,使用Rt-thread自带延时
	
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU9250
    MPU_Set_Gyro_Fsr(3);					        	//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					       	 	//加速度传感器,±2g
    MPU_Set_Rate(50);						       	 	//设置采样率50Hz
    MPU_Write_Byte(MPU9250_ADDR,MPU_INT_EN_REG,0X00);   //关闭所有中断
	MPU_Write_Byte(MPU9250_ADDR,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
	MPU_Write_Byte(MPU9250_ADDR,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_Write_Byte(MPU9250_ADDR,MPU_INTBP_CFG_REG,0X82);//INT引脚低电平有效，开启bypass模式，可以直接读取磁力计
    ID=MPU_Read_Byte(MPU9250_ADDR,MPU_DEVICE_ID_REG);  //读取MPU6500的ID
    if(ID==MPU6500_ID) //器件ID正确
    {
        MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X01);  	//设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT2_REG,0X00);  	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						       	//设置采样率为50Hz   
    }else return 1;
 
    ID=MPU_Read_Byte(AK8963_ADDR,MAG_WIA);    			//读取AK8963 ID   
    if(ID==AK8963_ID)
    {
        MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11);		//设置AK8963为单次测量模式
    }else return 1;

    return 0;
}

//设置MPU9250陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
rt_uint8_t MPU_Set_Gyro_Fsr(rt_uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU9250加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
rt_uint8_t MPU_Set_Accel_Fsr(rt_uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}

//设置MPU9250的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
rt_uint8_t MPU_Set_LPF(rt_uint16_t lpf)
{
	rt_uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU9250_ADDR,MPU_CFG_REG,data);//设置数字低通滤波器  
}

//设置MPU9250的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
rt_uint8_t MPU_Set_Rate(rt_uint16_t rate)
{
	rt_uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
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

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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

//得到磁力计值(原始值)
//mx,my,mz:磁力计x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
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
    MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963每次读完以后都需要重新设置为单次测量模式
    return res;
}

//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
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

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
rt_uint8_t MPU_Read_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf)
{ 
	rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, &reg, 1);
	rt_i2c_master_recv(mpu9250_i2c_bus, addr, 0, buf, len);
	
	return 0;
}

//IIC写一个字节 
//devaddr:器件IIC地址
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
rt_uint8_t MPU_Write_Byte(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t data)
{
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = data;
    rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, buf ,2);
	
	return 0;
}

//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
rt_uint8_t MPU_Read_Byte(rt_uint8_t addr,rt_uint8_t reg)
{
	rt_uint8_t buf;
	rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, &reg, 1);
	rt_i2c_master_recv(mpu9250_i2c_bus, addr, 0, &buf, 1);
	return buf;
}




















