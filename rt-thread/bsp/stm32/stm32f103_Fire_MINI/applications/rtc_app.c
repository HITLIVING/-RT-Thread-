#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

rt_err_t ret = RT_EOK;

static void set_time_app(int argc, char **argv)
{	
	/* 设置日期 */
    ret = set_date(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
    }

    /* 设置时间 */
    ret = set_time(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
    }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(set_time_app, rtc set time);

static int get_time_app(int argc, char *argv[])
{
    time_t now;   

    /* 获取时间 */
    now = time(RT_NULL);
    rt_kprintf("%s\n", ctime(&now));

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(get_time_app, rtc get time);

