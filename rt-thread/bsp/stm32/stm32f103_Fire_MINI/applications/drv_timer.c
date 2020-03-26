
#include <rtthread.h>
#include <rtdevice.h>

#include "gyroscope_app.h"
#include "drv_timer.h"


/*****************Extern Sensor Switch**********************/


/* ��ʱ����ʱ�ص����� */
static rt_err_t timer6_handle(rt_device_t dev, rt_size_t size)
{
	gyr_schedule();
    return 0;
}

rt_err_t timer6_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t timer6_dev = RT_NULL;   /* ��ʱ���豸��� */

    /* ���Ҷ�ʱ���豸 */
    timer6_dev = rt_device_find("timer6");
    if (timer6_dev == RT_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find timer6 device!\n");
		return ret;
    }

    /* �Զ�д��ʽ���豸 */
    ret = rt_device_open(timer6_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open timer6 device failed!\n");
		return ret;		
    }

    /* ���ó�ʱ�ص����� */
    rt_device_set_rx_indicate(timer6_dev, timer6_handle);

    /* ����ģʽΪ�����Զ�ʱ�� */
    rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD;
    ret = rt_device_control(timer6_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK)
    {
        rt_kprintf("timer6 set mode failed! ret is :%d\n", ret);
        return ret;
    }
	
	/* ���ö�ʱ����ʱֵ��������ʱ��*/
	rt_hwtimerval_t timeout_s;      
    timeout_s.sec = 0;      /* �� */
    timeout_s.usec = 5;     /* ΢�� */
    if (rt_device_write(timer6_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
    {
        rt_kprintf("timer6 set timeout value failed\n");
        return RT_ERROR;
    }
	
    return ret;
}












