
#include <rtthread.h>
#include <rtdevice.h>

#include "gyroscope_app.h"
#include "drv_timer.h"


/*****************Extern Sensor Switch**********************/


/* 定时器超时回调函数 */
static rt_err_t timer6_handle(rt_device_t dev, rt_size_t size)
{
	gyr_schedule();
    return 0;
}

rt_err_t timer6_init(void)
{
    rt_err_t ret = RT_EOK;
    rt_device_t timer6_dev = RT_NULL;   /* 定时器设备句柄 */

    /* 查找定时器设备 */
    timer6_dev = rt_device_find("timer6");
    if (timer6_dev == RT_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find timer6 device!\n");
		return ret;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(timer6_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open timer6 device failed!\n");
		return ret;		
    }

    /* 设置超时回调函数 */
    rt_device_set_rx_indicate(timer6_dev, timer6_handle);

    /* 设置模式为周期性定时器 */
    rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD;
    ret = rt_device_control(timer6_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK)
    {
        rt_kprintf("timer6 set mode failed! ret is :%d\n", ret);
        return ret;
    }
	
	/* 设置定时器超时值并开启定时器*/
	rt_hwtimerval_t timeout_s;      
    timeout_s.sec = 0;      /* 秒 */
    timeout_s.usec = 5;     /* 微秒 */
    if (rt_device_write(timer6_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
    {
        rt_kprintf("timer6 set timeout value failed\n");
        return RT_ERROR;
    }
	
    return ret;
}












